#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>



int main(int args,char** argv){
	
	int pid;
	int x,k;

	if(args !=3){
		perror("usage:argument");
		exit(1);
	}
	
	x=atoi(argv[1]); 
	k=atoi(argv[2]);

	if((matrices=shmget(IPC_PRIVATE,(k+1)*sizeof(float),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}


		int* p;

		if((p=shmat(matrices,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		p[k+1]=0;
		

	pid=fork();
	
	if(pid==0){
		int* t;
		int float;

		if((t=shmat(matrices,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		while(t[k+1]==0){
		}

		for(i=0;i<k;i+2){
			if(i==0)
				t[i]*=0;
			else
				t[i]*=1/i;	
		}
		

		for(i=0;i<k;i++){
			resultat+=t[i];
		}

		printf("%f",resultat);

		if(shmdt(t)==-1){
			perror("detachement \n");
			exit(3);
		}

		exit(0);
	}
	else{
		
		p[0]=1;

		for(i=1;i<k;i++){
			for(n=0;n<i;n++){
				p[i]*=x;	
		}

		t[k+1]=1;
		
		wait(NULL);

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
