/**
 * \file			exo3.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			24 octobre 2012
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
int count_sigint=0;
int i=2;					//valeur initial
long long terme;	//terme courant
int indice=0;			


long long alt_max=0;
int init_alt_max=0;

int vol=0;
int init_vol=0;

void syracuse(){
	while(terme!=1){
		

		if(terme%2==0){
			terme/=2;
			indice++;
		}
		else{
			terme*=3;
			terme++;
			indice++;
		}

		if(terme>alt_max){
			alt_max=terme;
			init_alt_max=i;
			}
		
		if(indice>vol){
			vol=indice;
			init_vol=i;
			}
		}	
}

	
void fin2(int sigalrm){
	if(count_sigint>=2)
		exit(0);
	else
		count_sigint=0;
}
	
void fin(int sigint){
	printf("plus grqnde valeur initiale %d,altitude max %lld avec %d comme valeur initiale,\n plus long vol %d avec  %d comme valeur initiale",i,alt_max,init_alt_max,vol,init_vol);

	count_sigint++;
	alarm(2);
}

void handler(int sigstop){

	printf("valeur initiale :%d , indice %d, terme courant %lld \n",i,indice,terme);
}

int main(int args,char* argv[]){
	
	
	struct sigaction act;
	struct sigaction stop;
	struct sigaction alarm;

	alarm.sa_handler=fin2;
	alarm.sa_flags=0;
	sigfillset(&alarm.sa_mask);

	act.sa_handler=handler;
	act.sa_flags=0;
	sigfillset(&act.sa_mask);

	stop.sa_handler=fin;
	stop.sa_flags=0;
	sigfillset(&stop.sa_mask);

	sigaction(SIGALRM,&alarm,NULL);
	sigaction(SIGINT,&stop,NULL);
	sigaction(SIGTSTP,&act,NULL);

	while(1){
		terme=i;
		indice=0;
		syracuse();
		i++;
		}

	exit(0);
}

		
