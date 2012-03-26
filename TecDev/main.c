#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "grille.h"


int main()
{
int x=2,y=2;
int decalX=0,decalY=0;
//int n,m;
initscr();
keypad(stdscr,1);
cbreak();
noecho();
int entree;

initscr();
//scanf("%d",&n);
//scanf("%d",&m);
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
       affiche_grille(l,decalX+2,decalY+2);
       //test2(l);
				refresh();
       if (entree==KEY_DOWN && y<(l->M)){
           y+=1;
           mvprintw(y,x,"O");
           }
       else if (entree == KEY_UP && y>0){
           y-=1;
           mvprintw(y,x,"O");
					 }
       else if (entree == KEY_RIGHT && x<(l->N)-1){
           x++;
           mvprintw(y, x,"O");
	   
           }
       else if (entree == KEY_LEFT && x>0){
           x-=1;
           mvprintw(y,x,"O");
            }
       else if (entree=='+')
           test->matrice[x][y-1]='+';
       else if (entree=='.')
           test->matrice[x][y-1]='.';
       else if (entree=='q'){
	    if (EstRempli(test)==1)
	      (mvprintw(15,10,"ERREUR: grille pas rempli"));
 	    else if (compare(l,test)==0)
	      (mvprintw(15,10,"Gagne !!!"));
	    else (mvprintw(15,10,"Perdu"));
	   }
        else if (entree=='l'){
        cocher_colonne(test,y,tab_col);
				}
         
        else if (entree=='c'){
           
        cocher_ligne(test,x,tab_lig);
         }   
         
  }

if (EstRempli(test)==1)
	(mvprintw(15,10,"ERREUR: grille pas rempli"));
 
if (compare(l,test)==0)
	(mvprintw(15,10,"Gagne !!!"));
	else (mvprintw(15,10,"Perdu"));

desalloue_grille(l);
desalloue_grille(test);
clear();
endwin();
return 0;
}
