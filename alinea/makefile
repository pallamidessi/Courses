CC=gcc
CFLAGS=-Wall -g

ALL:main

main:main.o tp1.o determinant.o gauss.o inversion.o resol.o
	$(CC) $^ -o $@ $(CFLAGS) 

main.o:main.c tp1.h	determinant.h
	$(CC) -c $< -o $@ $(CFLAGS) 

tp1.o:tp1.c tp1.h
	$(CC) -c $< -o $@ $(CFLAGS) 

determinant.o:determinant.c determinant.h
	$(CC) -c $< -o $@ $(CFLAGS) 

gauss.o:gauss.c gauss.h
	$(CC) -c $< -o $@ $(CFLAGS) 

inversion.o:inversion.c inversion.h
	$(CC) -c $< -o $@ $(CFLAGS) 

resol.o:resol.c resol.h
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm *.o main
