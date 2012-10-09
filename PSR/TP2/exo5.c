#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Liste{
	int *l;
	int nbrElem;
	int taille;

}str_liste,*liste;

liste agrandissement(liste a){
	realloc(a->l,a->taille+10*sizeof(int));
	a->taille+=10;
	return a;
	}

int* generateur(n){
	liste listeNbr=(liste)malloc(sizeof(str_liste));
	
	listeNbr->taille=(int)floor(pow(n,2))-1;
	int n2=listeNbr->taille;
	int i;
	listeNbr->l=(int*) malloc(n2*sizeof(int));

	liste->nbrElem=n2;

	for(i=0;i<n2;i++){
		liste[i]=i+1;
		}
	return listeNbr;
	}


int* diviseur(liste aDiviser,int d){
	liste nouvliste;
	

	for(i=0;i<aDiviser->taille;i++){
		if(aDiviser->l[i]%d!=0)
			if 
		
		
		}
	
	}
int main(int args,char* argv[])
{
	int n=0,m=0,pid;
	int i=0;
	int status;
  int pipefd;

	if(args !=2){
		perror("usage:nombre d'argument ");
		exit(1);
	}
	
	n=atoi(argv[1]);


pid=fork();

		pid=fork();		
		switch (pid){
			case -1 :
							perror("erreur fork");
							exit(2);
			case	0	:
							
							for(i=0;i<n;i++){
								if(fork())
									wait(&status);
								else
									break;
							}
			default : 		
								wait(&status);
	
		}
	}
