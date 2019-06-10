# bigdata_tools
tools for bigdata  
use mmap to load huge files at same time, very less memory usage.

# what

union_join: a tool for join two files to one, just combine each rows to one
for example:

fileA:  
AAAAA1  
AAAAA2

fileB:  
BBBBB1  
BBBBB2

joined:  
AAAAA1,BBBBB1  
AAAAA2,BBBBB2

ps. you can change a seperator which you like:   AAAAA1|BBBBB1

# benchmark

cat /dev/urandom | tr -dc "[:alnum:]" | fold -w 10 | head -n 100000000 > pw1.txt  
cat /dev/urandom | tr -dc "[:alnum:]" | fold -w 10 | head -n 100000000 > pw2.txt

time bin/union_join /tmp/pw1.txt /tmp/pw2.txt "|" | wc -l  
 100000000

real    0m29.741s  
user    0m25.098s  
sys     0m2.713s


cat /dev/urandom | tr -dc "[:alnum:]" | fold -w 10 | head -n 10000000 > pw1.txt  
cat /dev/urandom | tr -dc "[:alnum:]" | fold -w 10 | head -n 10000000 > pw2.txt

rikunoMacBook-puro:bigdata_tools itsusony$ time bin/union_join /tmp/pw1.txt /tmp/pw2.txt | wc -l  
 10000000

real	0m2.179s  
user	0m2.250s  
sys	0m0.138s

# how to build

make all

# author

meng.xiangliang1985@gmail.com
