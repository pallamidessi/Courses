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
	nettoye[i]=motlexique[i];			
}
return nettoye;
}



int arrondiAneuf(int brut){
return (brut/9)*9;				
}



int main (int argc , char* argv[] ){

char *verification="";
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
		milieu=lseek(fd,,SEEK_END);


while((signe=strcmp(argv[1],verification))){
			if(signe>0){ 		
				debut=milieu;
				milieu=lseek(fd,arrondiAneuf((debut+fin)/2),SEEK_CUR);
			}
			else{ 
				fin=milieu;
				milieu=lseek(fd,arrondiAneuf((debut+fin)/2),SEEK_CUR);
			}
				read(fd,verification,6);
				verification=nettoyer(verification);
				printf("%s",verification);
}

return 0;
}
