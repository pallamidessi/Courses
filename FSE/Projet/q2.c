#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "e2fs.h"

#define	MAXBUF	100

/* Lit un bloc physique quelconque en passant par un buffer */

int main (int argc, char *argv [])
{
    ctxt_t c ;
		int i;
		buf_t tmp;

    if (argc < 3)
    {
	fprintf (stderr, "usage: %s fs blkno ... blkno\n", argv [0]) ;
	exit (1) ;
    }

    c = e2_ctxt_init (argv [1], MAXBUF) ;
    if (c == NULL)
    {
	perror ("e2_ctxt_init") ;
	exit (1) ;
    }

    /* A REDIGER */
		for(i=2;i<argc;i++){
			tmp=e2_buffer_get(c,atoi(argv[i]));
				
			write(1,e2_buffer_data(tmp),e2_ctxt_blksize(c));
			e2_buffer_put(c,tmp);
			printf("\n");
		}
		 
		e2_buffer_stats(c);
		
    e2_ctxt_close (c) ;

    exit (0) ;
}
