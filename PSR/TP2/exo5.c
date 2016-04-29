/**
 * \file			exo5.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			16 octobre 2012
 * \brief		
 * 
 * \details		
 * 
 */ 


/*L'exercice n'est pas fonctionnelle*/
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


int main(int args,char* argv[])
{
	int n=0,i=0;
	int status;
	int buffer;
	int nr;
	int diviseur;
	if(args !=2){
		perror("usage:nombre d'argument ");
		exit(1);
	}
	
	n=atoi(argv[1]);

	int ncarre=(int)floor(pow(n,2));

	int n_fils=0;
	n_fils++;

	int pipefd[2];
	pipe(pipefd);

  int pipefd2[2];
	pipe(pipefd2);

	while(!fork()){


			if(n_fils==n) 														//Cas d'arret dernier fils possible 
				exit(0);
			if (n_fils==1){
				
				close(pipefd2[1]);
				close(pipefd2[0]);
				close(pipefd[0]);
				
				for(i=2;i<=ncarre;i++){
					buffer=i;
					write(pipefd[1],&buffer,4);
				}

				close(pipefd[1]);
			}
			else if((n_fils%2)==0){											//lit dans pipefd et ecrit dans pipefd2
					
				close(pipefd2[0]);
				close(pipefd[1]);
				
				if((nr=read(pipefd[0],&diviseur,4))>0)    // Cas d'arret : le pipe est vide
					printf(" %d ",diviseur);
				else 
					exit(0);

				while((nr=read(pipefd[0],&buffer,4))>0){
					if((buffer%diviseur)!=0)
						write(pipefd2[1],&buffer,nr);
				
				}
				close(pipefd[0]);
				close(pipefd2[1]);
				
			}
			else if((n_fils%2)==1){											//lit dans pipefd2 et ecrit dans pipefd
				
				close(pipefd2[1]);
				close(pipefd[0]);
				
				if((nr=read(pipefd2[0],&diviseur,4))>0)    // Cas d'arret : le pipe est vide
					printf(" %d ",diviseur);
				else 
					exit(0);

				while((nr=read(pipefd2[0],&buffer,4))>0){
					if((buffer%diviseur)!=0)
						write(pipefd[1],&buffer,nr);
				}
				
				close(pipefd2[0]);
				close(pipefd[1]);
			}

	n_fils++;		
	}

wait(&status);

	return 0;
}

