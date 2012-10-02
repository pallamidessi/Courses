
/**
 * \file			exo4b.c
 * \author		Pallamidessi joseph
 * \version		1.0 
 * \date			2 octobre 2012
 * \brief			La version bufferisee de getchar2().
 * 
 * \details		Le buffer (liste de char) et ses indices sont definie en static	
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
	char static buffer[1024];				
	int fi;
	static int indice;
	static nbr_elem;


	if(nbr_elem==0){
		nbr_elem=read(0,&buffer,1024);
		indice=0;
	}
	else{ 
		indice++;
		nbr_elem--;
		return buffer[indice--];
	}
}


int main(int argc, char *argv[])
{
  char c;

  while((c = getchar())!=EOF)
    putchar(c);
}
