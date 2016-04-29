#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>


#define T_BUFFER 2
int main (int argc , char* argv[] ){

int nr;
char buffer[T_BUFFER];

while((nr=read(0,buffer,T_BUFFER))!=-1){
	write(1,buffer,nr);			
}

return 0;
}
