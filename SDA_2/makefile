CC=gcc
CFLAGS= -Wall -g

main:main.o arbin.o
	$(CC) -o test $^ $(CFLAGS)

main.o:main.c arbin.h
	$(CC) -c >$ $(CFLAGS)

arbin.o: arbin.c arbin.h 
	$(CC) -c >$ $(CFLAGS)

