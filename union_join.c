#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define FFLUSH_ROW_COUNT 1000000

void usage() {
    fprintf(stderr,"usage:\n\t./union_join FILE_LEFT FILE_RIGHT FILE_OUTPUT {SEPERATOR} (files left and right must readable, seperator default use ',')\n\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    //prepare

    if (argc < 4) usage();

    char* filepathl = strdup(argv[1]);
    char* filepathr = strdup(argv[2]);
    char* filepath_out = strdup(argv[3]);
    char* separator = argv[4] ? strdup(argv[4]) : NULL;
    if (!separator) separator = strdup(",");


    struct stat sbl,sbr;

    if (stat(filepathl, &sbl) != 0 || stat(filepathr, &sbr) != 0)
        usage();

    int fdl,fdr;
    char *mapl, *mapr;
    long page_size, map_sizel, map_sizer;
 
    fdl = open(filepathl, O_RDONLY);
    fdr = open(filepathr, O_RDONLY);
    if(fdl < 0 || fdr < 0) {
        printf("Error : can't open left file or right file\n");
        return -1;
    }
 
    page_size = getpagesize();
    map_sizel = (sbl.st_size / page_size + 1) * page_size;
    map_sizer = (sbr.st_size / page_size + 1) * page_size;

    mapl = (char*)mmap(NULL, map_sizel, PROT_READ, MAP_SHARED, fdl, 0);
    mapr = (char*)mmap(NULL, map_sizer, PROT_READ, MAP_SHARED, fdr, 0);
    if (mapl == MAP_FAILED || mapr == MAP_FAILED) {
        printf("Error : mmap left or right failed\n");
        return -1;
    }




    // main logic

    char *pl = mapl, *pr = mapr, *ptl = NULL, *ptr = NULL;
    char sep = '\n';

    FILE* fo = fopen(filepath_out, "w");
    int i=0;
    while(1) {
        ptl = strchr(pl+1, sep);
        ptr = strchr(pr+1, sep);
        if (ptl && ptr) {
            char L[1024] = "", R[1024] = "";
            long lenL = ptl - pl, lenR = ptr - pr;

            strncpy(L,pl,lenL);
            strncpy(R,pr,lenR);

            fprintf(fo, "%s%s%s\n", L, separator, R);
            if (i++ >= FFLUSH_ROW_COUNT) {
                fflush(fo);
                i=0;
            }

            pl += lenL+1;
            pr += lenR+1;
        } else {
            break;
        }
    }
    fflush(fo);
    fclose(fo);




    // close everything

    close(fdl);
    close(fdr);
    munmap(mapl, map_sizel);
    munmap(mapr, map_sizer);

    return 0;
}
