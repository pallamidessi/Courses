OBJ=obj/
SRC=src/
BIN=bin/
INC=-Iheader
VPATH=src:header:bin
CC=gcc
CFLAGS= -Wall -g -lm  

ALL: main

main:main.o arbq1.o arbq.o couleur.o iobmp.o bmpfile.o
	$(CC)  -o $(BIN)$@ $(OBJ)main.o $(OBJ)arbq1.o $(OBJ)arbq.o $(OBJ)couleur.o $(OBJ)iobmp.o $(OBJ)bmpfile.o $(CFLAGS)

main.o:main.c arbq1.h iobmp.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

bmpfile.o:bmpfile.c bmpfile.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)
	
iobmp.o:iobmp.c iobmp.h bmpfile.h base.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

arbq.o: arbq.c arbq.h 
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

arbq1.o: arbq1.c arbq1.h arbq.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

couleur.o: couleur.c couleur.h 
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

clean:
	rm -r obj/*.o bin/main Doxydoc obj/strasbourg_matrix_*

doxy:
	doxygen

