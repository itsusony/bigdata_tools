all:
	gcc -Wall -O3 -o bin/union_join union_join.c
	gcc -Wall -O3 -pthread -o bin/realtime_logsum realtime_logsum.c
