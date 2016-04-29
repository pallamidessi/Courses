

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

int main(int args,char* argv[])
{
	int n=0,m=0,pid;
	int i=0;

	if(args !=3){
		perror("usage:nombre d'argument ");
		exit(1);
	}
	
	n=atoi(argv[1]);
	m=atoi(argv[2]);

	for(i=0;i<n;i++){	
			pid=fork();

			switch (pid){
				case -1 :
								perror("erreur fork");
								exit(2);
				case	0	: 
								exit(0);
				default : for (i=0;i<n;i++){
										fork();
										printf("%d ",getpid());
									}
									exit(0);
			}
	}
	



	return 0;
																																								 ;
}
