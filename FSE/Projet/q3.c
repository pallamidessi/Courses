#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/fs.h>
#include <linux/ext2_fs.h>

#include "e2fs.h"

#define	MAXBUF	100

/* Affiche le numero de bloc de l'inode n et la taille du fichier */

int main (int argc, char *argv [])
{
    ctxt_t c ;
		pblk_t pb;
		buf_t tmp;
		struct ext2_inode* in;

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

    /* A REDIGER */
		pb=e2_inode_to_pblk(c,atoi(argv[2]));
		tmp=e2_buffer_get(c,pb);

		printf("L'inode est dans le bloc physique %d\n",pb);
		in=e2_inode_read(c,atoi(argv[2]),tmp);
		
		int test=in->i_blocks*512;
		printf("taille du fichier en octet : %d \n",test);
		e2_buffer_put(c,tmp);

    e2_ctxt_close (c) ;

    exit (0) ;
}
