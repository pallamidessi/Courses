
/**
 * \file			exo2b.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			2 octobre 2012
 * \brief			On lit le fichier binaire cree pour l'exo2	
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


if ((fd=open("test.txt",O_RDONLY))==-1){
	exit(1);
}

for(i=0;i<10;i++){
	read(fd,&buffer,4);			// On lit par pack de 4 octets (sizeof(int))	
	printf("%d\n",buffer);
	}

close(fd);
exit(0);
}		
