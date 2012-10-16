
/**
 * \file			exo8.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			16 octobre 2012
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
#include <string.h>

#define N 20
#define M 50


/*transforme une chaine en une structure contenant un tableaux de mot,et une indication
 * pour savoir s'il faut executer la commande  suivante directement*/
typedef struct Commande{
	char** cmd;
	int cmd_suivante;
	} str_commande,*commande;

commande decoupage(char* chaine){
	
	int i=0,j=0,l=0,exec=0;
	
	commande tab=(commande) malloc (sizeof(str_commande));

	tab->cmd=(char**) malloc(N*sizeof(char*));	

	for(i=0;i<N;i++){
		tab->cmd[i]=(char*) malloc(M*sizeof(char));	
	}

	tab->cmd_suivante=0;

	i=0,j=0,l=0;							//on decoupe la cahine en mot (separateur " ",fin '\0')
	while(chaine[i]!='\n'){
		exec=1;
		if(chaine[i]=='&'){				// le cas cmd&
			tab->cmd_suivante=1;
			i++;
			exec=0;
		}

		if(chaine[i]==32){
			tab->cmd[j][l]='\0';
			j++;
			i++;
			l=0;
			
			if(chaine[i]=='\n')
				exec=0;
		}

		if(exec==1){
			tab->cmd[j][l]=chaine[i];
			l++;
			i++;
		}
	}




	tab->cmd[j][l]='\0';
	j+=1;
	tab->cmd[j]=(char*) NULL;

	j-=1;
	if((strcmp(tab->cmd[j],"&"))==0){ 		//le cas cmd &
		printf("le test est vrai");
		tab->cmd_suivante=1;
		tab->cmd[j]=NULL;
	}
		
	return tab;
	}


int main(int args,char* argv[]){
int i;
commande mot_decoupe;
FILE* pIN=fdopen(1,"r");	
char entree[256];
pid_t pid_fils;
int status;

	while(1){
		fgets(entree,256,pIN);	
		mot_decoupe=decoupage(entree);
		if(mot_decoupe->cmd_suivante==0){
			if((pid_fils=fork())==0){
				execvp(mot_decoupe->cmd[0],mot_decoupe->cmd);
				printf("erreur\n");
				break;
			}
			else
				wait(NULL);
			}
			else{
				if(fork()==0){
					execvp(mot_decoupe->cmd[0],mot_decoupe->cmd);
					printf("erreur\n");
					break;
				}
			else
				waitpid(pid_fils,&status,WNOHANG);									//On fait un wait non bloquant
			}
				for(i=0;i<N;i++)
					free(mot_decoupe->cmd[i]);
			
				free(mot_decoupe);
		}
	return 0;	
}
