#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "grille.h"


int main()
{
int interrupteur=0;
int x=2,y=2;
//int n,m;
initscr();
keypad(stdscr,1);
cbreak();
//noecho();
int entree;

initscr();
//scanf("%d",&n);
//scanf("%d",&m);
grille l=alloue_grille(5,5);
grille test=alloue_grille(5,5);

affiche_grille(l);
printw("\n\n");

refresh();

l=charger_grille(l, "toto.txt", "r");
affiche_grille(l);

refresh();
clear();

while  ((entree=getch())!='a'){
    clear();
       //compter_colonne(l);
       //compter_ligne(l);
       //affiche_grille(test);
       test2(l);
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
          if (interrupteur==0){
           cocher_ligne(test,y-1,interrupteur);
           interrupteur=1;
           }
           else {
           cocher_ligne(test,y-1,interrupteur);
           interrupteur=0;
           }
        } 
        else if (entree=='c'){
          if (interrupteur==0){
           cocher_ligne(test,x,interrupteur);
           interrupteur=1;
           }
           else {
           cocher_ligne(test,x,interrupteur);
           interrupteur=0;
           }
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
