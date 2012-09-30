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
int nr;
int i=0;

char buffer[T_BUFFER];

if (argc != 2){
	printf("usage : on donne un fichier a lire");
	exit(1);
	}

fd = open(argv[1],O_RDONLY);

while((nr=read(fd,buffer,T_BUFFER))>0){				
	
	for(i=0;i<nr;i++){
		buffer[i]=toupper(buffer[i]);
	}
	
	write(1,buffer,nr);				
}

exit(0);
}
