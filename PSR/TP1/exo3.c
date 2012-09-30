#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>



int main (int argc , char* argv[] ){
				
int fd;
int truc;

if (argc != 3){
	printf("usage : on donne le nom et lq taille du fichier que l'on souhaite");
	exit(1);
	}
fd=open(argv[1],O_CREAT|O_TRUNC|O_WRONLY,0777);

write(fd,&truc,atoi(argv[2]));

close(fd);

exit(0);
}
