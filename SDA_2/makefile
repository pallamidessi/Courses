CC=gcc
CFLAGS= -Wall -g

main:main.o arbin.o
	$(CC)  -o  $@ $^ $(CFLAGS)

main.o:main.c arbin.h
	$(CC) -c $< -o $@  $(CFLAGS)

arbin.o: arbin.c arbin.h 
	$(CC) -c $< -o $@ $(CFLAGS)

