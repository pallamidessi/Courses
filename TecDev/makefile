CC=gcc
TARGET = logigraphe
CFLAGS= -Wall -g
LDFLAGS=

ALL= 

$(TARGET): main.o grille.o Image.o
	$(CC)  -o main main.o grille.o Image.o -lncurses

main.o: main.c grille.h 
	$(CC) -Wall -g -c main.c $(CFLAGS)
	

grille.o:grille.c  
	$(CC) -Wall -g -c grille.c $(CFLAGS)
	
Image.o:Image.c
	$(CC) -Wall -g -c Image.c $(CFLAGS)

clean:
	rm -r *.o main
	
doxy: 
	doxygen 
