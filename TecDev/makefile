VPATH = src:header:obj
OBJ = obj/
SRC = src/
CC = gcc
TARGET = logigraphe
CFLAGS = -Wall -g
LDFLAGS = -lncurses
LDFLAGS2 = -lcairo 
INC =-Iheader -I/usr/include/cairo -I/usr/local/include -L/usr/local/lib 

all:$(TARGET)_ncurses $(TARGET)_cairo 

$(TARGET)_cairo: main_cairo.o grille_cairo.o Image_cairo.o compteur.o cairo_util.o
	$(CC) -o $@ $(OBJ)main_cairo.o $(OBJ)grille_cairo.o $(OBJ)Image_cairo.o $(OBJ)compteur.o $(OBJ)cairo_util.o $(LDFLAGS2)

$(TARGET)_ncurses: main.o grille.o Image.o compteur.o
	$(CC) -o $@ $(OBJ)main.o $(OBJ)grille.o $(OBJ)Image.o $(OBJ)compteur.o $(LDFLAGS)

main.o: main.c grille.h Image.h 
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS) 

main_cairo.o: main_cairo.c grille_cairo.h Image_cairo.h 
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS) 
	
grille.o:grille.c grille.h 
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS)

cairo_util.o:cairo_util.c cairo_util.h
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS)

grille_cairo.o:grille_cairo.c grille.h 
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS)
	
Image.o:Image.c Image.h grille.h
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS)

Image_cairo.o:Image_cairo.c Image_cairo.h grille_cairo.h
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS)

compteur.o:compteur.c compteur.h
	$(CC) -c  $< $(INC) -o $(OBJ)$@ $(CFLAGS)
	
clean:
	rm -r $(OBJ)*.o logigraphe_cairo logigraphe_ncurses
	
doxy: 
	doxygen  
