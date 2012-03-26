#include<stdio.h>
#include<stdlib.h>
#include"grille.h"
#include<ncurses.h>


grille alloue_grille(int n,int m){
int i,v;

grille L=NULL;
L=malloc(sizeof(str_grille));
L->matrice= malloc((n*sizeof(char*)));
	
	for (i=0;i<n;i++)
		L->matrice[i]=malloc(m*sizeof(char));

	for (i=0;i<n;i++){
    for (v=0;v<m;v++){
    L->matrice[i][v]='.';
    }
	}

L->N=n;
L->M=m;

return L;
}


void affiche_grille(grille L,int decalX,int decalY){
int i, v ;
int originX=decalX;

  for(i=0;i<L->N;i++){
		decalX=originX;
    for(v=0;v<L->M;v++){
      mvprintw(decalY-1,decalX,"%c", (L->matrice[i][v]));
			decalX++;
      }
    decalY++;
}

}

void desalloue_grille(grille L){
int i=0;

for(i=0;i<L->N;i++)
	free(L->matrice[i]);
	free(L);
}

grille charger_grille(grille L, char* nom,char* mode){
int i=0,v=0;
grille charge=alloue_grille(L->N,L->M);
FILE* fichier = NULL;
char chaine[TAILLE_MAX] = "";

fichier = fopen(nom, mode);

	if (fichier != NULL)
  {
 		 while (fgets(chaine, TAILLE_MAX, fichier) != NULL )
     {
     	 for(i=0;i<L->M;i++)
       charge->matrice[v][i]=chaine[i];
			 v++;
	   }

  	 fclose(fichier);
  }
return charge;
}

int EstRempli(grille l){
int i=0,v=0;

for(i=0;i<l->N;i++)
	for(v=0;v<l->M;v++){
	  if (l->matrice[i][v] ==' ')
	    return 1;				
	}
return 0;
}
				
int compare(grille l1,grille l2){
int i=0,v=0;

for(i=0;i<l1->N;i++)
	for(v=0;v<l1->M;v++){
	  if (l1->matrice[i][v] !=l2->matrice[i][v])
	    return 1;				
	}
return 0;
}


grille compter_ligne(grille l){
grille valeur=alloue_grille(l->N,l->M);
int i=0,v=0;
int x=0;


	for(i=0;i<l->N;i++)
		for(v=0;v<l->M;v++)
	  	valeur->matrice[i][v]=0;

	for(i=0;i<l->N;i++){
		x=0;
		for(v=0;v<l->M;v++){
	  	if (l->matrice[i][v]=='+')
	  	  valeur->matrice[i][x]+=1;
	  	else if ((valeur->matrice[i][x]!=0) && (l->matrice[i][v]=='.'))
	    	x++;							
		}
	}
return valeur;
}

int count_decalX(grille l){
int i=0,v=0,tmp=0,valeur=0;

for(i=0;i<l->N;i++){
	if (tmp<valeur)
		tmp=valeur;
		valeur=0;
	for(v=0;v<l->M;v++){
	  	if (l->matrice[i][v]!=0)
	    valeur++;
	}
}	
return tmp;

}
int count_decalY(grille l){
int i=0,v=0,tmp=0,valeur=0;

for(i=0;i<l->M;i++){
	if (tmp<valeur)
		tmp=valeur;
		valeur=0;
	for(v=0;v<l->N;v++){
	  	if (l->matrice[v][i]!=0)
	    valeur++;
	}
}	
return tmp;
	}

grille compter_colonne(grille l){
grille valeur=alloue_grille(l->N,l->M);
int i=0,v=0;
int x=0;



for(i=0;i<l->N;i++)
	for(v=0;v<l->M;v++)
	  valeur->matrice[i][v]=0;

for(i=0;i<l->M;i++){
	x=0;
	for(v=0;v<l->N;v++){
	  	if (l->matrice[v][i]=='+')
	    valeur->matrice[x][i]+=1;
	  else if ((valeur->matrice[x][i]!=0) && (l->matrice[v][i]=='.'))
	    x++;							
				}
}
return valeur;
}	    


void afficheCountLigne(grille l , int decalX ,int decalY){
int i=0,v=0;	
int originX=decalX;

	for(i=0;i<l->N;i++){
		decalY++;
		decalX=originX;
   	for(v=0;v<l->M;v++){
	  	if (l->matrice[i][v]!=0)
	    	mvprintw(decalY,decalX,"%d",l->matrice[i][v]);
				decalX++;
	   }  
	}
}

void afficheCountCol(grille l , int decalX ,int decalY){
int i=0,v=0;
int originY=decalY;

for(i=0;i<l->M;i++){
  refresh();
	for(v=0;v<l->N;v++){
	  if (l->matrice[v][i]!=0){
			mvprintw(decalY,decalX,"%d",l->matrice[v][i]);
			decalY++;
		}
	}
	decalY=originY;
	decalX++;
}
}

void cocher_colonne(grille l,int y,int* tab){
int i;

  if (tab[y]==0){
    for(i=0;i<l->N;i++)
  		l->matrice[i][y]='.';
		tab[y]=1;	
  } 	
  else {  
  for(i=0;i<l->N;i++)
  	l->matrice[i][y]='+';
		tab[y]=0;	
}
}

void cocher_ligne(grille l,int y,int* tab){
int i;

  if (tab[y]==0){
    for(i=0;i<l->M;i++)
  	l->matrice[y][i]='.';
		tab[y]=1;	
  } 	
  else {  
  for(i=0;i<l->M;i++)
  	l->matrice[y][i]='+';
		tab[y]=0;	
}
}

void test2(grille l){
int i;

    for(i=0;i<(l->M);i++)
  	printw("%c",l->matrice[1][i]);
}

