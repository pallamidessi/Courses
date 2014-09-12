CFLAGS = -Wall -g -O0
#OBJECTS = $(patsubst src/%.c, %.o, $(wildcard src/*.c))
#OBJECTS_DIR = $(patsubst src/%.c, objets/%.o, $(wildcard src/*.c))
CC = gcc
LIB=-lm -fopenmp
LIB_PATH=/usr/lib/SDL
INC_PATH = include
SRC_PATH = src
OBJ_PATH = obj
BIN_PATH = bin
VPATH = src:include:objets:bin

ALL: Kmeans 

Kmeans: main.o kmeans.o
	gcc $(OBJECT) -o $@ $(OBJ_PATH)/kmeans.o $(OBJ_PATH)/main.o -lm -fopenmp
	mv $@ bin/.
	
main.o: main.c
	gcc $(OBJECT) -o $@ -c $<  -I $(INC_PATH) -fopenmp
	mv $@ obj/.

kmeans.o: Kmeans.c
	gcc $(CFLAGS) -o $@ -c $<  -I $(INC_PATH) -fopenmp
	mv $@ obj/.

#%.o : %.c
#	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)
#	mv $@ obj/.


clean : 
	rm $(OBJ_PATH)/*.o $(BIN_PATH)/main 
	rm *~ $(INC_PATH)/*~ $(SRC_PATH)/*~
