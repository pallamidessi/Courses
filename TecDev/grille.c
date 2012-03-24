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
    L->matrice[i][v]=' ';
    }
}

L->N=n;
L->M=m;

return L;
}


void affiche_grille(grille L){
int i, v ;
  for(i=0;i<L->M;i++){
    printw("\n");
    for(v=0;v<L->N;v++)
      printw("%c", (L->matrice[v][i]));
      
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
             charge->matrice[i][v]=chaine[i];

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

/*
void compter_colonne(grille l){
int valeur[l->N][l->M];
int i=0,v=0;
int x=0;


for(i=0;i<l->N;i++)
	for(v=0;v<l->M;v++)
	  valeur[i][v]=0;

for(i=0;i<l->N;i++)
	for(v=0;v<l->M;v++){
	  if (l->matrice[i][v]=='+')
	    valeur[i][x]+=1;
	  else if ((valeur[i][x]!=0) && (l->matrice[i][v]=='.'))
	    x++;							
				}
for(i=0;i<l->M;i++){
printw("\n");
   for(v=0;v<l->N;v++){
	  if (valeur[i][v]!=0)
	    printw("%d",valeur[i][v]);
	    
	    }
	    
	    
}
}
*/

/*
void compter_ligne(grille l){
int valeur[l->N][l->M];
int i=0,v=0;
int x=0;


for(i=0;i<l->N;i++)
	for(v=0;v<l->M;v++)
	  valeur[i][v]=0;

for(i=0;i<l->N;i++)
	for(v=0;v<l->M;v++){
	  if (l->matrice[i][v]=='+')
	    valeur[x][i]+=1;
	  else if ((valeur[x][i]!=0) && (l->matrice[i][v]=='.'))
	    x++;							
				}
for(i=0;i<l->M;i++){
printw("\n");
   for(v=0;v<l->N;v++){
	  if (valeur[i][v]!=0)
	    printw("%c",valeur[i][v]);
	    
	    }
	    
	    
}
}
*/

void cocher_ligne(grille l,int y,int x){
int i;

  if (x==0){
    for(i=0;i<l->N;i++)
  	l->matrice[i][y]='.';
  } 	
  else {  
  for(i=0;i<l->N;i++)
  	l->matrice[i][y]='+';
}
}

void cocher_colonne(grille l,int y,int x){
int i;

  if (x==0){
    for(i=0;i<l->M;i++)
  	l->matrice[y][i]='.';
  } 	
  else {  
  for(i=0;i<l->M;i++)
  	l->matrice[y][i]='+';
}
}
void test2(grille l){
int i;

    for(i=0;i<(l->M);i++)
  	printw("%c",l->matrice[1][i]);
}

