


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
	int pipefd[2];
	pipe(pipefd);
	char buffer;
	int octet=0;

	while(1){
		write(pipefd[1],&buffer,1);
		octet++;
		printf("%d\n",octet);
		}
		close(pipefd[1]);
	return 0;
	
	}
