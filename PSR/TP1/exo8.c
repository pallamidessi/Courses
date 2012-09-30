#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>



char* nettoyer(char* motlexique){

char *nettoye=(char*)malloc(6*sizeof(char));
int i;

for(i=1;i<6;i++){
	nettoye[i-1]=motlexique[i];			
}
nettoye[5]='\0';
free(motlexique);
return nettoye;
}



int arrondiAneuf(int brut){
return (brut/9)*9;				
}



int main (int argc , char* argv[] ){

char *verification=(char*) malloc(6*sizeof(char));
int milieu=0,debut=0,fin=0;
int fd;
int signe;

if (argc!=2){
	printf("usage: nombre d'argument\n");
	exit(1);
}

if ((fd=open("lexique",O_RDONLY))==-1){
	printf("le fichier n'existe pas ou probleme de droit");
	exit(2);
} 

	  debut=0;
		fin=lseek(fd,0,SEEK_END);		
		milieu=lseek(fd,-(arrondiAneuf(fin/2)),SEEK_END);

				read(fd,verification,6);
				lseek(fd,-6,SEEK_CUR);
				verification=nettoyer(verification);
				
while((signe=strcmp(argv[1],verification))){
			if(signe>0){ 		
				debut=milieu;
				milieu=lseek(fd,arrondiAneuf((debut+fin)/2),SEEK_SET);
			}
			else{ 
				fin=milieu;
				milieu=lseek(fd,(arrondiAneuf((debut+fin)/2)),SEEK_SET);
			}
				read(fd,verification,6);
				lseek(fd,-6,SEEK_CUR);
				verification=nettoyer(verification);
				printf("%s\n",verification);
}
free(verification);
return 0;
}
