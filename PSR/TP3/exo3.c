/**
 * \file			exo3.c
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
#include <time.h>

int c=0;


void fin(int alarme){
	if(c>=2)
		exit(0);
	else
		c=0;
}

void handler(int sig){
	int n,m;

	n=rand()%6+1;
	m=rand()%6+1;
	printf("%d %d \n",n,m);
	c++;
	alarm(1);
}

int main(int args,char* argv[]){
	
	srand(time(NULL));
	
	struct sigaction act;
	struct sigaction alarm;


	act.sa_handler=handler;
	act.sa_flags=0;
	sigfillset(&act.sa_mask);

	alarm.sa_handler=fin;
	alarm.sa_flags=0;
	sigfillset(&alarm.sa_mask);

	sigaction(SIGALRM,&alarm,NULL);
	sigaction(SIGINT,&act,NULL);

	while(1){}

	exit(0);
}

		
