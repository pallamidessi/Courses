CFLAGS = -Wall -g -O0
CC = gcc
LDFLAGS=-lm 
INC_PATH = include
SRC_PATH = src
OBJ_PATH = objets
BIN_PATH = bin
VPATH = src:include:objets:bin

ALL: main

main : entropy.o bmpfile.o main.o
	gcc $(CFLAGS) -o $@ $(OBJ_PATH)/entropy.o $(OBJ_PATH)/main.o $(OBJ_PATH)/bmpfile.o $(LDFLAGS) 
	@mv $@ bin/.

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)
	@mv $@ objets/.

clean : 
	rm $(OBJ_PATH)/*.o $(BIN_PATH)/main 
	@rm *~ $(INC_PATH)/*~ $(SRC_PATH)/*~
