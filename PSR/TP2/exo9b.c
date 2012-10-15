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


int main(int args,char* argv[]){
	
	int fd;
	
		fd=open(argv[args-1],O_RDONLY);

		argv[args-1]=NULL;
		
		if(fork()==0){
			close(0);
			dup(fd);
			execvp(argv[1],&argv[1]);
			printf("erreur\n");
			exit(1);
		}
		else
			wait(NULL);
			
	return 0;	
}
	
