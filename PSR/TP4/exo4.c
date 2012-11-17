#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>

#define N 15



int main(){
	
	int i,j,k;
	int pid;
	int matrices;
	srand(time(NULL));

	if((matrices=shmget(IPC_PRIVATE,3*(N*sizeof(int))*(N*sizeof(int)),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}


	pid=fork();
	
	if(pid==0){
		int* t;

		if((t=shmat(matrices,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		int (*A)[N][N];
		int (*B)[N][N];
		int (*C)[N][N];
		A=(int (*)[N][N])&t[0];
		B=(int (*)[N][N])&t[N*N];
		C=(int (*)[N][N])&t[2*(N*N)];

		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				(*B)[i][j]=rand()%100;	
			}
		}

		(*C)[N][N]=1;
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

		int (*A)[N][N];
		int (*B)[N][N];
		int (*C)[N][N];
		A=(int (*)[N][N])&p[0];
		B=(int (*)[N][N])&p[N*N];
		C=(int (*)[N][N])&p[2*(N*N)];

		
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				(*A)[i][j]=rand()%100;	
			}
		}

		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				(*C)[i][j]=0;	
			}
		}
		
			
		while((*C)[N][N]!=1){
		}

		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				for(k=0;k<N;k++){
					(*C)[i][j]=(*A)[i][k]*(*B)[k][j];	
				}
			}
		}
		
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				printf("%d ",(*C)[i][j]);
			}
			printf("\n");
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
