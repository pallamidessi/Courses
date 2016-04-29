
/**
 * \file			exo7.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			2 octobre 2012
 * \brief			On ecrit sur la sortie standard le contenu d'un fichier donne en argument.
 * 
 * \details		
 * 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>


int main (int argc , char* argv[] ){

	int fd;
	int i=0;
	char buffer;

	if (argc!=2){
		printf("usage: nombre d'argument\n");
		exit(1);
	}

	if ((fd=open(argv[1],O_RDONLY))==-1){
		printf("le fichier n'existe pas ou probleme de droit");
		exit(2);
	} 

	while(lseek(fd,-i, SEEK_END)!=-1){
		read(fd,&buffer,1);
		write(1,&buffer,1);				
		i++;
	}

	return 0;
}
