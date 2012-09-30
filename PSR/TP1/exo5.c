#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>


int main (int argc , char* argv[] ){

struct stat statFichier;
int i=0;
char *type="";
char droit[9];
stat(argv[1],&statFichier);

if (argc!=2){
	printf("usage: nombre d'argument\n");
	exit(1);
}

for(i=0;i<8;i++){
	droit[i]='-';				
}

droit[9]='\0';


if(S_ISREG(statFichier.st_mode))
	type="fichier ordinaire";
else if(S_ISDIR(statFichier.st_mode))
	type="un repertoire";
else if(S_ISLNK(statFichier.st_mode))
	type="un lien symbolique";
else 
	type="inconnu";


if(S_IRUSR & (statFichier.st_mode))
	droit[0]='r';
if(S_IWUSR & (statFichier.st_mode))
	droit[1]='w';
if(S_IXUSR & (statFichier.st_mode))
	droit[2]='x';


	if(S_IRGRP & (statFichier.st_mode))
		droit[3]='r';
	if(S_IWGRP & (statFichier.st_mode))
		droit[4]='w';
	if(S_IXGRP & (statFichier.st_mode))
		droit[5]='x';

	if(S_IROTH & (statFichier.st_mode))
		droit[6]='r';
	if(S_IWOTH & (statFichier.st_mode))
		droit[7]='w';
	if(S_IXOTH & (statFichier.st_mode))
		droit[8]='x';

printf("%s   type:%s   protection :%s \n",argv[1],type,droit);
return 0;
}
