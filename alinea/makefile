OBJ=obj/
SRC=src/
BIN=bin/
INC=-Iinclude
VPATH=src:include:bin
CC=gcc
CFLAGS= -Wall -g 

ALL:main 

main:main.o tp1.o determinant.o gauss.o inversion.o resol.o 
	$(CC) -o $(BIN)$@ $(OBJ)main.o $(OBJ)tp1.o $(OBJ)determinant.o $(OBJ)gauss.o $(OBJ)inversion.o $(OBJ)resol.o $(CFLAGS)

main.o:main.c tp1.h	determinant.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

tp1.o:tp1.c tp1.h 
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

determinant.o:determinant.c determinant.h 
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

gauss.o:gauss.c gauss.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

inversion.o:inversion.c inversion.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

resol.o:resol.c resol.h
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)


clean:
	rm -r obj/*.o bin/main DocDoxy

doxy:
	doxygen

