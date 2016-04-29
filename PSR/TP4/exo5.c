#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

int tableau_partage;
float* p;
	
/*On recupere le SIGINT pour detruire les shared memory avant de quitter*/
void trap(int SIG){
	
		if(shmdt(p)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmctl(tableau_partage,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}

		exit(0);
}

int main(int args,char** argv){
	
	int pid;
	int k;
	int i,n;
	float x;
	struct sigaction act;

	act.sa_handler=trap;
	act.sa_flags=0;
	sigfillset(&act.sa_mask);

	if(args !=3){
		perror("usage:argument");
		exit(1);
	}
	
	x=atof(argv[1]); 
	k=atoi(argv[2]);

	/* Creation du tableau et du semaphore en memoire partage */
	if((tableau_partage=shmget(IPC_PRIVATE,(k+1)*sizeof(float),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}

	sigaction(SIGINT,&act,NULL);


	/* Attachement dans le pere */
	if((p=shmat(tableau_partage,NULL,0))==(void*)-1){
		perror("attachement \n");
		exit(2);
	}

	p[k+1]=0;
		
	pid=fork();
	
	if(pid==0){													// Processu fils
		float* t;
		float resultat=0.0;

		/* Attachement dans le fils */
		if((t=shmat(tableau_partage,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		/* Boucle infinie pour la synchro */
		while(t[k+1]==0){
			sleep(0.01);					//pour eviter de faire tourner le processeur pour rien
		}
		
		/*le fils multiplie les puissance paires par le coefficient approprie negatif*/
		
		t[0]*=0;								//cas particulier pour eviter le test dans la boucle
		
		for(i=2;i<k;i+=2){
				t[i]*=(float)-1/i;	
		}
		

		/* Addition du resultat */
		for(i=0;i<k;i++){
			resultat+=t[i];
		}

		printf("%f",resultat);

		/* Detachement dans le fils */
		if(shmdt(t)==-1){
			perror("detachement \n");
			exit(3);
		}

	}
	else{
		
		p[0]=1;

		/*le pere remplis le tableau partage par les puissances de x*/
		for(i=1;i<k;i++){
			p[i]=x;
			for(n=0;n<i-1;n++){
				p[i]*=x;	
			}
		}

		/*le pere multiplie les puissance impaire par le coefficient approprie negatif*/
		
		for(i=3;i<k;i+=2){
				p[i]*=(float)1/i;	
		}
		
		/* Le pere signifie au fils qu'il a fini en modifie le booleen place en fin de tableau*/
		p[k+1]=1;
		
		wait(NULL);

		/* Detachement dans le pere */
		if(shmdt(p)==-1){
			perror("detachement \n");
			exit(3);
		}

		/* destruction dans le pere */
		if(shmctl(tableau_partage,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}
	
	}
	return 0;
}
