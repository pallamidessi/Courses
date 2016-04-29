#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "e2fs.h"

#define	MAXBUF	100

/* Lit un bloc physique quelconque (sans passer par les buffers) */

int main (int argc, char *argv [])
{
    ctxt_t c ;

    if (argc != 3)
    {
	fprintf (stderr, "usage: %s fs blkno\n", argv [0]) ;
	exit (1) ;
    }

    c = e2_ctxt_init (argv [1], MAXBUF) ;
    if (c == NULL)
    {
	perror ("e2_ctxt_init") ;
	exit (1) ;
    }
		void* data=malloc(e2_ctxt_blksize(c));

    /* A REDIGER */
		e2_block_fetch(c,atoi(argv[2]),data);
		
		write(1,data,e2_ctxt_blksize(c));
   	e2_ctxt_close (c) ;
		free(data);
    exit (0) ;
}
