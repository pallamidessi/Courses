/**
 * \file       logigraphe.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief      logigraphe.c .
 *
 * \details    Contient les fonction relative au logigraphe ou a leurs traitements.
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "grille.h"
#include "Image.h"
#include "compteur.h"



int main()
{
	int x=2,y=2;
	int n=0,m=0;
	int deja=0;
	int decalX=0,decalY=0;
	initscr();
	keypad(stdscr,1);
	cbreak();
	//noecho();
	int entree=0,choix=0;
	char nom_fichier[20];
	grille test,l;
	initscr();

//compteur(decalY,decalX);

while(choix==0){
	if(deja==0){
	printw("F:charger un fichier\n");
	printw("I:charger une image\n");
	deja=1;
	}

	//refresh();
  entree=getch();
	if (entree==('f')){
		printw("Nom du fichier(defaut : toto.txt)\n");
		refresh();
		scanw("%s",nom_fichier);

		printw("Taille du fichier (largeur hauteur)\n");
		refresh();
		scanw("%d %d",&n,&m);

		l=alloue_grille(n,m);
		test=alloue_grille(n,m);
		
		l=charger_grille(l, nom_fichier, "r");
	  choix=1;
	}else 
		if (entree==('i')){
		printw("Nom de image(defaut : couleur.ppm)\n");
		refresh();
		scanw("%s",nom_fichier);
    
		l=charger_image(nom_fichier,"r",Seuil(nom_fichier));
		test=alloue_grille(l->N,l->M);
		refresh();
		choix=1;
		}
}		
int tab_col[test->N];          //Pour la func cocher ligne
int tab_lig[test->M];          //Pour la func cocher colonne


grille valL=compter_ligne(l);
grille valC=compter_colonne(l);

decalX=count_decalX(valL);
decalY=count_decalY(valC);



while  ((entree=getch())!='a'){
    clear();
       afficheCountCol(valC,decalX+2,0);
       afficheCountLigne(valL,0,decalY);
       affiche_grille(test,decalX+2,decalY+2);
			refresh();
      deplacement(entree,tab_col,tab_lig,&x,&y,decalX,decalY,test,l);
      mvprintw(5+decalY+(l->N),5+decalX,"q : pour verifie son logigraphe");
			mvprintw(6+decalY+(l->N),5+decalX,"c : coche/decoche la colonne courante");
			mvprintw(7+decalY+(l->N),5+decalX,"l : coche/decoche la ligne courante");
			mvprintw(8+decalY+(l->N),5+decalX,"a : pour quitter le programme");
			mvprintw(9+decalY+(l->N),5+decalX," + ou . : rentre + ou . ");
}



desalloue_grille(l);
desalloue_grille(test);
clear();
endwin();
return 0;
}
