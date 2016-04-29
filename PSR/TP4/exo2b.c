
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
	
	if((int_partage=shmget(ftok("exo1.c",0),sizeof(int),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}

	if((t=shmat(int_partage,NULL,0))==(void*)-1){
		perror("attachement\n");
		exit(2);
	}

	*t=0;
	
	while(1){
		if((*t)==1){
			printf("le programme 1  a lus la valeur 1\n");
			break;
		}
	}

	if(shmdt(t)==-1){
		perror("detachement\n");
		exit(3);
	}

	if(shmctl(int_partage,IPC_RMID,NULL)==-1){
		perror("destruction de la memoire partagee\n");
		exit(4);
	}

	return 0;
}
