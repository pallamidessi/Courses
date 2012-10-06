
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

int main(){
	
	int pipefd[2],n;

	if(args !=2){
		perror("usage:nombre d'argument ");
		exit(1);
	}

	n=atoi(argv[1]);
	
	for(i=0;i<n;i++){	
		pipe(pipefd);
		int m=1;
		pid=fork();
			

			switch (pid){
				case -1 :
								perror("erreur fork");
								exit(2);
				case	0	: write()
								
				default : for (i=0;i<n;i++){
										printf("%d ",i);
										exit(0);
									}
			}
		}
