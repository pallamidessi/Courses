main: main.o grille.o Image.o Image.h grille.h 
	gcc -Wall -g -o main main.o grille.o Image.o -lncurses

main.o: main.c grille.h 
	gcc -Wall -g -c main.c -lncurses
	

grille.o:grille.c grille.h 
	gcc -Wall -g -c grille.c -lncurses
	
Image.o:Image.c Image.h 
	gcc -Wall -g -c Image.c -lncurses

clean:
	rm -r *.o main
	rm main
	
doxy: 
	doxygen 
