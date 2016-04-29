/**
 * \file			exo1.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			2 octobre 2012
 * \brief			Fonction getchar2,fonctionnant comme getchar() mais non bufferisee
 * 
 * \details		elle revoit un int, pour pouvoir revoyer un EOF si elle n'arrive pas a lire
 * 
 * 
 */ 




#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>



int  getchar2(){
	char buffer;				
	int nr=0;


	nr=read(0,&buffer,1);

	if(nr==0)
		return EOF;
	else 
		return buffer;
}


int main (int argc , char* argv[] ){
			
  char c;

  while((c = getchar())!=EOF)
    putchar(c);
exit(0);
}

