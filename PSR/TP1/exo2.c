
/**
 * \file			exo2.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			2 octobre 2012
 * \brief			On creer un fichier (ou on ecrase le fichier existant), et on ecrit 10 int
 * dedans
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




#define T_BUFFER 256

int main (int argc , char* argv[] ){

	int fd;
	int buffer;
	int i=0;


	fd=open("test.txt",O_CREAT|O_TRUNC|O_WRONLY,0777);

	for(i=0;i<10;i++){
		scanf("%d",&buffer);
		write(fd,&buffer,4);	
	}

	close(fd);
	exit(0);
}
				
