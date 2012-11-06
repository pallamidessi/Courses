/**
 * \file			exo5.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			06 novembre 2012
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


int pipefd[2];
int buffer;
struct sigaction send;
struct sigaction received;

void send_random(int sig1,siginfo_t *siginfo,void* context){
	
	buffer=rand()%100;
	write(pipefd[1],&buffer,sizeof(int));
	
	if(kill(siginfo->si_pid,SIGUSR2)==-1){
		perror("kill");
		exit(0);
	}
}

void read_pipe(int sig2){
	read(pipefd[0],&buffer,sizeof(int));
	fflush(stdin);
	printf("%d\n",buffer);
	sleep(5);
}

int main(int args,char* argv[]){
	
	srand(time(NULL));
	int nbr_fils=0;
	int i=0;

	if(args!=2){
		perror("usage:nombre d'argument");
	}
	
	nbr_fils=atoi(argv[1]);

	
	send.sa_sigaction=send_random;
	send.sa_flags=SA_SIGINFO;
	sigfillset(&send.sa_mask);

	received.sa_handler=read_pipe;
	received.sa_flags=0;
	sigfillset(&received.sa_mask);
	
	sigaction(SIGUSR1,&send,NULL);
	sigaction(SIGUSR2,&received,NULL);

	pipe(pipefd);

	while(i<nbr_fils){
		if(fork()==0){
			while(1){
				sleep(2);
				if(kill(getppid(),SIGUSR1)==-1){
					perror("kill");
					exit(1);
				}
			}
			exit(0);
		}
		i++;	
	}

	while(1){
		sleep(1);
	}
	
	return 0;
}
