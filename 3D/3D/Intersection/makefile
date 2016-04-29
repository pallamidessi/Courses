OBJ=obj/
SRC=src/
BIN=bin/
INC=-Iinclude
VPATH=src:include:bin
CC=gcc
CFLAGS= -Wall -g 

ALL:Intersection Intersection_mask

Intersection:Intersection.o  
	$(CC) -o $(BIN)$@ $(OBJ)Intersection.o  $(CFLAGS)

Intersection_mask:Intersection_mask.o  
	$(CC) -o $(BIN)$@ $(OBJ)Intersection_mask.o  $(CFLAGS)

Intersection.o:Intersection.c 
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

Intersection_mask.o:Intersection_mask.c
	$(CC) -c $< $(INC) -o $(OBJ)$@ $(CFLAGS)

clean:
	rm -r obj/*.o bin/Intersection bin/Intersection_mask DocDoxy

doxy:
	doxygen

