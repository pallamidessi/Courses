main: main.o grille.o grille.h 
	gcc -Wall -g -o main main.o grille.o -lncurses

main.o: main.c grille.h 
	gcc -Wall -g -c main.c -lncurses
	

grille.o:grille.c grille.h 
	gcc -Wall -g -c grille.c -lncurses
	
clean:
	rm -r *.o main
