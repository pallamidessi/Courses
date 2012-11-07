#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>



int main(){

	int int_partage;	
	int pid;

	if((int_partage=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}


	pid=fork();
	
	if(pid==0){
		int* t;
		
		if((t=shmat(int_partage,NULL,0))==(void*)-1){
			perror("attachement dans le fils\n");
			exit(2);
		}

		*t=1;

		if(shmdt(t)==-1){
			perror("detachement dans le fils\n");
			exit(3);
		}

		exit(0);
	}
	else{
		int* p;

		if((p=shmat(int_partage,NULL,0))==(void*)-1){
			perror("attachement dans le pere\n");
			exit(2);
		}

		*p=0;
		while(1){
			if((*p)==1){
				printf("le pere a lus la valeur 1\n");
				
				if(shmdt(p)==-1){
					perror("detachement dans le pere\n");
					exit(3);
				}

				if(shmctl(int_partage,IPC_RMID,NULL)==-1){
					perror("destruction de la memoire partagee\n");
					exit(4);
				}

				break;
			}
		}	
	}
	
	wait(NULL);	
	return 0;
}
