#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#include "e2fs.h"

#define	MAXBUF	100


/* Affiche le fichier reference par l'inode avec l'"ouverture de fichier" */
int main (int argc, char *argv [])
{
    ctxt_t c ;

    if (argc != 3)
    {
			fprintf (stderr, "usage: %s fs inode\n", argv [0]) ;
			exit (1) ;
    }

    c = e2_ctxt_init (argv [1], MAXBUF) ;
    if (c == NULL)
    {
			perror ("e2_ctxt_init") ;
			exit (1) ;
    }
		
		char last_char;

    file_t f=e2_file_open(c,atoi(argv[2]));

		while(( last_char = e2_file_getc(f) ) != -1 ){
			printf("%c",last_char);

		}	
		
		printf("\n");
		e2_file_close(f);
    e2_ctxt_close (c) ;

    exit (0) ;
}
