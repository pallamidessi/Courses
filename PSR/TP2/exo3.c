/**
 * \file			exo3.c
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

int main(int args,char* argv[])
{
	int n=0,m=0,pid;
	int i=0,j=0;
	int status;

	if(args !=3){
		perror("usage:nombre d'argument ");
		exit(1);
	}
	
	n=atoi(argv[1]);
	m=atoi(argv[2]);

	for(j=0;j<m;j++){	
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
							printf("proc %d pere %d\n",getpid(),getppid()); //L'affichage est desordonne, mais si on dessine l'arbre des pid ,on voit bien qu'un arbre de N fils et de profondeur M a ete cree
							exit(0);
			default : 		
								wait(&status);
	
		}
	}
	return 0;
																																								 
}
