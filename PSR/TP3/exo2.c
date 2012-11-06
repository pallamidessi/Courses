/**
 * \file			exo2.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			6 novembre 2012
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
#include <signal.h>
struct sigaction act;
int i;
int n;

void handler(int sig){
	i=n;
}

int main(int args,char* argv[]){
	

	act.sa_handler=handler;
	act.sa_flags=0;
	sigfillset(&act.sa_mask);

	sigaction(SIGINT,&act,NULL);

	if(args !=2){
		perror("usage:nombre d'argument ");
		exit(1);
	}

	
	n=atoi(argv[1]);

	for(i=n;i>=0;i--){
		sleep(1);
		printf("%d\n",i);
	}
	
	exit(0);
}

		
