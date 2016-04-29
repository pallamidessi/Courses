/**
 * \file			exo9.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			16 octobre 2012
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


int main(int args,char* argv[]){
	
	int fd;
	
		 /*on ouvre le fichier donne en derniere argument(on le cree,ou on l'ecrase)*/
		fd=open(argv[args-1],O_WRONLY|O_CREAT|O_TRUNC,0777);   

		/*on fait pointer le dernier element du tableau d'argument sur null, pour la fonction
		 * execvp*/
		argv[args-1]=NULL;
		
		if(fork()==0){
			close(1);														//on ferme la sortie standard
			dup(fd);														//on dup le fichier vers lequelle on veut rediriger l'entree
			execvp(argv[1],&argv[1]);
			printf("erreur\n");
			exit(1);
		}
		else
			wait(NULL);
			
	return 0;	
}
	
	
	
