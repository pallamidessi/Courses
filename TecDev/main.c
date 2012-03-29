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


int main()
{
int x=2,y=2;
//int n=0,m=0;
int decalX=0,decalY=0;
initscr();
keypad(stdscr,1);
cbreak();
noecho();
int entree;

initscr();

//printw("taille du logigraphe dans le fichier (ligne x colonne)");
//scanf("%d",&n);
//refresh();
//scanf("%d",&m);
//refresh();
grille l=alloue_grille(5,50);
grille test=alloue_grille(5,50);
int tab_col[test->N];          //Pour la func cocher ligne
int tab_lig[test->M];          //Pour la func cocher colonne


l=charger_grille(l, "toto.txt", "r");
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
