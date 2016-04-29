#include <stdio.h>
#include <stdlib.h>

#include "e2fs.h"

#define	MAXBUF	100


/* Affiche le fichier ou le repertoire reference par l'inode */
int main (int argc, char *argv [])
{
    ctxt_t c ;

    if (argc != 3)
    {
			fprintf (stderr, "usage: %s fs chemin\n", argv [0]) ;
			exit (1) ;
    }

    c = e2_ctxt_init (argv [1], MAXBUF) ;
    if (c == NULL)
    {
			perror ("e2_ctxt_init") ;
			exit (1) ;
    }
		
		int nb_inode=e2_namei(c,argv[2]);
		
		printf("inode %d \n",nb_inode);

    e2_ctxt_close (c) ;

    exit (0) ;
}
