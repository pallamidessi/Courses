
/**
 * \file			exo8.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			2 octobre 2012
 * \brief			On fait une recherche dichotomique dans un ficher lexique fournis, et on
 * affiche la recherche. 	
 * 
 * \details		L'utilisateur doit donne en argument une chine en majuscule.		
 * 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


/*Fonction pour enlever les " et , des chaine donnees*/
char* nettoyer(char* motlexique){

char *nettoye=(char*)malloc(6*sizeof(char));
int i;

for(i=1;i<6;i++){
	nettoye[i-1]=motlexique[i];			
}
nettoye[5]='\0';
free(motlexique);
return nettoye;
}


/*Permet de se placer correctement sur le debut d'un mot (guillement compris)*/
int arrondiAneuf(int brut){
return (brut/9)*9;				
}



int main (int argc , char* argv[] ){

	char *verification=(char*) malloc(6*sizeof(char));
	int milieu=0,debut=0,fin=0;
	int fd;
	int signe;

	if (argc!=2){
		printf("usage: nombre d'argument\n");
		exit(1);
	}
	
	if ((fd=open("lexique",O_RDONLY))==-1){
		printf("le fichier n'existe pas ou probleme de droit");
		exit(2);
	} 

	/*initialisation de la recherche,on place le debut ,le milieu et la fin */
	  debut=0;
		fin=lseek(fd,0,SEEK_END);		
		milieu=lseek(fd,-(arrondiAneuf(fin/2)),SEEK_END);

		read(fd,verification,6);
		lseek(fd,-6,SEEK_CUR);
		verification=nettoyer(verification);
				

	/*Boucle de recherche*/
	while((signe=strcmp(argv[1],verification))){   /*Cas d'arret : on a trouve le mot */
		if(signe>0){ 		
			debut=milieu;
			milieu=lseek(fd,arrondiAneuf((debut+fin)/2),SEEK_SET);
		}
		else{ 
			fin=milieu;
			milieu=lseek(fd,(arrondiAneuf((debut+fin)/2)),SEEK_SET);
		}
			read(fd,verification,6);
			lseek(fd,-6,SEEK_CUR);
			verification=nettoyer(verification);
			printf("%s\n",verification);
		
		if(milieu==fin || milieu==debut){		/*Si le mot n'appartient pas au lexique, autre condition d'arret de la recherche*/
			printf("%s ne fait pas parti du lexique\n",argv[1]);
			strcpy(verification,argv[1]);    /*permet d'arreter la boucle*/
		}

	}
	close(fd);
	free(verification);
	return 0;
}
