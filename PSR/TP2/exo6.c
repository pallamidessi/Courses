

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

int i,j;	
	
for(i=0;i<2;i++){
	if(fork()==0){
		if(i==0){
			printf("premier fils\n");
			execlp("ls","ls","-l",NULL);
		}
		else{
			printf("deuxieme fils\n");
			execlp("ps","ps","-l",NULL);
		}
	}
	else{
		for(j=0;j<2;j++)
			wait(NULL);
		}
	}
	
return 0;
}

