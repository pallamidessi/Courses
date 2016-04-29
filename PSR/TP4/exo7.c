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

typedef int bool;
#define TRUE 1
#define FALSE 0

int tableau_partage,semaphore,p_turn,p_wantIn;
float *p,*t;
int *index_partage,*turn,*wantIn;


/*On recupere le SIGINT pour detruire les shared memory avant de quitter*/
void trap(int SIG){
	
		if(shmdt(p)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(turn)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(wantIn)==-1){
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

		if(shmctl(p_turn,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}

		if(shmctl(p_wantIn,IPC_RMID,NULL)==-1){
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

	/* Creation du tableau,du semaphore en memoire et des variable necessaire a l'algoritme
	 * de Peterson en memoire partage */
	if((tableau_partage=shmget(IPC_PRIVATE,(k)*sizeof(float),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}

	if((semaphore=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}
	
	if((p_turn=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666))==-1){
		perror("creation de la memoire partage\n");
		exit(1);
	}

	if((p_wantIn=shmget(IPC_PRIVATE,2*sizeof(bool),IPC_CREAT|0666))==-1){
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

	if((turn=shmat(p_turn,NULL,0))==(void*)-1){
		perror("attachement \n");
		exit(2);
	}

	if((wantIn=shmat(p_wantIn,NULL,0))==(void*)-1){
		perror("attachement \n");
		exit(2);
	}
	
	/* Initialisation du tableau pour l'algorithme de Peterson et du semaphore */
	wantIn[0]=FALSE;
	wantIn[1]=FALSE;
	*index_partage=0;
	
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
		
		if((turn=shmat(p_turn,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		if((wantIn=shmat(p_wantIn,NULL,0))==(void*)-1){
			perror("attachement \n");
			exit(2);
		}

		/*le fils remplis le tableau partage par les puissances paire de x*/

		int myPid=1;
		int otherPid=0;
		while(*index_partage<k){

			wantIn[myPid]=TRUE;
			*turn=otherPid;
			/*section critique -------------------------------*/
			while (!(wantIn[otherPid]==FALSE || *turn==myPid)){
				sleep(0.01);
			}
			
			if(*index_partage%2==1){
				p[*index_partage]=p[(*index_partage)-1]*x;
				(*index_partage)++;
			
			}
			/*fin section critique -------------------------------*/
			wantIn[myPid]=FALSE;
		}

		
		/*le fils multiplie les puissance paires par le coefficient approprie negatif*/
		t[0]*=0;						//cas particulier pour eviter le test dans la boucle
		
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

		if(shmdt(turn)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(wantIn)==-1){
			perror("detachement \n");
			exit(3);
		}
	
	}
	else{
		
		/*le pere remplis le tableau partage par les puissances paire de x*/
	
		int myPid=0;
		int otherPid=1;
		
		while(*index_partage<k){
			
			wantIn[myPid]=TRUE;
			*turn=otherPid;
			
			while (!(wantIn[otherPid]==FALSE || *turn==myPid)){
				sleep(0.01);
			}

			/*section critique -------------------------------*/
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
			/*fin section critique -------------------------------*/
			wantIn[myPid]=FALSE;
		}

		
		/*le pere multiplie les puissance impaire par le coefficient approprie positif*/
		for(i=3;i<k;i+=2){
				p[i]*=(float)1/i;	
		}

		
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

		if(shmdt(turn)==-1){
			perror("detachement \n");
			exit(3);
		}

		if(shmdt(wantIn)==-1){
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

		if(shmctl(p_wantIn,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}

		if(shmctl(p_turn,IPC_RMID,NULL)==-1){
			perror("destruction de la memoire partagee\n");
			exit(4);
		}
	}
	return 0;
}
