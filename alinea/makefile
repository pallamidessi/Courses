CC=gcc
FLAGS=-Wall-g

ALL:main

main:main.o tp1.o
	$(CC) $< -o $@ $(CFLAGS) 

main.o:main.c tp1.h
	$(CC) -c $< -o $@ $(CFLAGS) 

tp1.o:tp1.c tp1.h
	$(CC) -c $< -o $@ $(CFLAGS) 
