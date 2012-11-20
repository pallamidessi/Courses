#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>


int tableau_partage=0,semaphore=0;
float* p;
float* t;
int* index_partage;

/*On recupere le SIGINT pour detruire les shared memory avant de quitter*/
void trap(int SIG){
	
		if(shmdt(p)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(index_partage)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmctl(semaphore,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
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

	if((semaphore=shmget(IPC_PRIVATE,sizeof(int ),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}
	
	sigaction(SIGINT,&act,NULL);

	/* Attachement dans le pere */
	if((p=shmat(tableau_partage,NULL,0))==(void*)-1){
		perror("attachement \n");
		exit(2);
	}

	if((index_partage=shmat(semaphore,NULL,0))==(void*)-1){
		perror("attachement \n");
		exit(2);
	}
	
	/* Initialisation du booleen de test et du semaphore */
	*index_partage=0;
	p[k+1]=0;
		

	pid=fork();
	
	if(pid==0){																// Processus fils
		float resultat=0.0;


		/* Attachement dans le fils */
		if((t=shmat(tableau_partage,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}
	
		if((index_partage=shmat(semaphore,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}
		
		/* Boucle en ecoute active sur le semaphore,pour remplir les puissance impaire dans le
		 * tableau */
		while(*index_partage<k){
			if(*index_partage%2==1){
				p[*index_partage]=p[(*index_partage)-1]*x;
				(*index_partage)++;
			}
		}
		
		/* Boucle infinie pour la synchro */
		while(t[k+1]==0){
			sleep(0.01);								//pour eviter de faire tourner le processeur pour rien
		}
		
		/*le fils multiplie les puissance paires par le coefficient approprie negatif*/
		
		t[0]*=0;											//cas particulier pour eviter le test dans la boucle
		
		for(i=2;i<k;i+=2){
				t[i]*=(float)-1/i;	
		}
		
		/* Calcul du resultat */
		for(i=0;i<k;i++){
			resultat+=t[i];
		}

		printf("%f\n",resultat);

		/* Detachement dans le fils */
		if(shmdt(t)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(index_partage)==-1){
			perror("detachement \n");
			exit(3);
		}
	
	}
	else{																		  // Processus pere
		
		/* Boucle en ecoute active sur le semaphore, pour remplir les puissance paire dans le
		 * tableau */
		while(*index_partage<k){
			if(*index_partage%2==0){
				if(*index_partage==0){
					p[0]=1;
					(*index_partage)++;
				}
				else{
					p[*index_partage]=p[*index_partage-1]*x;
					(*index_partage)++;
				}
			}
		}
		

		/*le pere multiplie les puissance impaire par le coefficient approprie negatif*/
		
		for(i=3;i<k;i+=2){
				p[i]*=(float)1/i;	
		}

		p[k+1]=1;
		
		wait(NULL);

		/* Detachement dans le pere */
		if(shmdt(p)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(index_partage)==-1){
			perror("detachement \n");
			exit(3);
		}

		/* destruction dans le pere */
		if(shmctl(semaphore,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}

		if(shmctl(tableau_partage,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}
	}
	return 0;
}
