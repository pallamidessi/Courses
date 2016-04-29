CFLAGS = -Wall -g -O0
CC = g++
LDFLAGS=-lm 
INC_PATH = include
SRC_PATH = src
OBJ_PATH = objets
BIN_PATH = bin
VPATH = src:include:objets:bin

ALL: main

main : entropy.o  main.o quicksort.o genetic.o
	$(CC) $(CFLAGS) -o $@ $(OBJ_PATH)/entropy.o $(OBJ_PATH)/main.o  $(OBJ_PATH)/quicksort.o $(OBJ_PATH)/genetic.o $(LDFLAGS) 
	@mv $@ bin/.

%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)
	@mv $@ objets/.

clean : 
	rm $(OBJ_PATH)/*.o $(BIN_PATH)/main 
	@rm *~ $(INC_PATH)/*~ $(SRC_PATH)/*~
