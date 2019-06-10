#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_POOLSIZE 10000 * 10000
#define BUF_SIZE 1024000

char** pool=NULL;
unsigned long i=0;
pthread_mutex_t poollock = PTHREAD_MUTEX_INITIALIZER;

void init_pool() {
    pool = malloc(MAX_POOLSIZE * sizeof(char*));
    i=0;
}

void *stream_read(void *p) {
    char buf[BUF_SIZE];
    while(fgets(buf, BUF_SIZE, stdin) != NULL) {
        char* new = strdup(buf);
        pthread_mutex_lock(&poollock);
        pool[i++] = new;
        pthread_mutex_unlock(&poollock);
    }
    return 0;
}
void *stream_process(void *p) {
    while (1) {
        char** _pool=NULL;
        unsigned long _i=0;
        pthread_mutex_lock(&poollock);
        if (i>0) {
            _pool = pool;
            _i = i;
            init_pool();
        }
        pthread_mutex_unlock(&poollock);

        if (_i > 0) {
            fprintf(stderr,"%ld\n", _i);
            unsigned long j;for (j=0;j<_i;j++) {
                char* str = _pool[j];
                free(str);
            }
            free(_pool);
        }

        sleep(1);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    init_pool();

    pthread_t pr,pp;

    pthread_create(&pr, NULL, &stream_read, NULL);
    pthread_create(&pp, NULL, &stream_process, NULL);

    pthread_join(pr,NULL);
    pthread_join(pp,NULL);
    return 0;
}
