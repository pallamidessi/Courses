#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>


int main (int argc , char* argv[] ){
			
char buffer;				
int fi;

fi = open(0,O_RDONLY);

read(0,&buffer,1);
close(fi);

exit(0);
}
