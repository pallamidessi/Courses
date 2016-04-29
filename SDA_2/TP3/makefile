CC=gcc
CFLAGS= -Wall -g

ALL: main 

main:main.o rel.o
	$(CC)  -o  $@ $^ $(CFLAGS)

main.o:main.c rel.h
	$(CC) -c $< -o $@  $(CFLAGS)

rel.o: rel.c rel.h 
	$(CC) -c $< -o $@ $(CFLAGS)

