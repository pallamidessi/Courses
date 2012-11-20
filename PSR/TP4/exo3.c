#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>

#define i 5
#define j 5


int main(){
	
	int k;
	int pid;
	int matrices;
	srand(time(NULL));

	if((matrices=shmget(IPC_PRIVATE,3*(i*sizeof(int))*(j*sizeof(int))+sizeof(int),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}


	pid=fork();
	
	if(pid==0){
		int* t;
		
		sleep(1);						//pour garantir que les nombres sont bien tirer aleatoirement
		srand(time(NULL));

		if((t=shmat(matrices,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		for(k=i*j;k<2*(i*j);k++){
			*(t+k)=rand()%100;	
		}
		
		*(t+3*(i*j))=1;
		if(shmdt(t)==-1){
			perror("detachement \n");
			exit(3);
		}

		exit(0);
	}
	else{
		int* p;

		if((p=shmat(matrices,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		
		for(k=0;k<i*j;k++){
			*(p+k)=rand()%100;	
		}
		
		//*(p+3*(i*j))=0;
			
		while((*(p+3*(i*j)))!=1){
		}
		
		for(k=2*(i*j);k<3*(i*j);k++){
			*(p+k)=*(p+(k-(2*(i*j))))+*(p+(k-(i*j)));	
		}
		
		
		for(k=0;k<(i*j);k++){
			if(k%j==0)
				printf("\n");

			printf("%d ",*(p+k));
		}
		
		printf("\n\n");
		
		for(k=(i*j);k<2*(i*j);k++){
			if(k%j==0)
				printf("\n");

			printf("%d ",*(p+k));
		}
		
		printf("\n\n");
		
		for(k=2*(i*j);k<3*(i*j);k++){
			if(k%j==0)
				printf("\n");

			printf("%d ",*(p+k));
		}
		
		if(shmdt(p)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmctl(matrices,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}
	}
	
	return 0;
}
