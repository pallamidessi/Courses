
#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>



int main(){

	int int_partage;	
	int* t;

	sleep(1);
	
	if((int_partage=shmget(ftok("exo1.c",0),sizeof(int),0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}

	if((t=shmat(int_partage,NULL,0))==(void*)-1){
		perror("attachement\n");
		exit(2);
	}

	*t=1;

	if(shmdt(t)==-1){
		perror("detachement\n");
		exit(3);
	}

	return 0;
}
