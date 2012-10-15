
/**
 * \file			exo2.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			3 octobre 2012
 * \brief		
 * 
 * \details		
 * 
 */ 
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define N 20
#define M 50

char** decoupage(char* chaine){
	
	int i=0,j=0,l=0,exec=0;
	char** liste_mot=(char**) malloc(N*sizeof(char*));	

	for(i=0;i<N;i++){
		liste_mot[i]=(char*) malloc(M*sizeof(char));	
	}


	i=0,j=0,l=0;
	while(chaine[i]!='\n'){
		exec=1;


		if(chaine[i]==32){
			liste_mot[j][l]='\0';
			j++;
			i++;
			l=0;
			
			if(chaine[i]=='\n')
				exec=0;
		}

		if(exec==1){
			liste_mot[j][l]=chaine[i];
			l++;
			i++;
		}
	}




	liste_mot[j][l]='\0';
	j+=1;
	liste_mot[j]=(char*) NULL;
	return liste_mot;
	}


int main(int args,char* argv[]){
int i;
char** mot_decoupe;
FILE* pIN=fdopen(1,"r");	
char entree[256];

	while(1){
		fgets(entree,256,pIN);	
		mot_decoupe=decoupage(entree);
		if(fork()==0){
			execvp(mot_decoupe[0],mot_decoupe);
			printf("erreur\n");
			break;
			}
		else
			wait(NULL);
			
			for(i=0;i<N;i++)
				free(mot_decoupe[i]);
			
			free(mot_decoupe);
		}
	return 0;	
}
