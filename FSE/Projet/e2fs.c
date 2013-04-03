#include <stdio.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "e2fs.h"

struct buffer
{
    void *data ;			/* les donnees du bloc */
    pblk_t blkno ;			/* numero de bloc physique */
    int valid ;				/* donnees valides */
    struct buffer *next ;
} ;

struct context
{
    int fd ;
    struct ext2_super_block sb ;
    int ngroups ;			/* nombre de groupes dans gd [] */
    struct ext2_group_desc *gd ;	/* c'est un tableau */
    /* ce qui suit concerne les lectures bufferisees */
    struct buffer *last ;		/* pointe sur dernier buffer */
    int bufstat_read ;			/* nombre de demandes de lecture */
    int bufstat_cached ;		/* nombre de lectures en cache */
} ;

struct ofile
{
    struct context *ctxt ;		/* eviter param a chaque e2_file_xxx */
    struct buffer *buffer ;		/* buffer contenant l'inode */
    struct ext2_inode *inode ;		/* l'inode proprement dit */
    lblk_t curblk ;			/* position en bloc */
    char *data ;			/* donnees */
    int len ;				/* longueur utile dans les donnees */
    int pos ;				/* position dans les donnees */
} ;

/******************************************************************************
 * Initialisation du contexte
 */

ctxt_t e2_ctxt_init (char *file, int maxbuf)
{
	int i=0;
	int fd_file=0;
	int nb_group;
	int block_size;
	buf_t tmp;
	ctxt_t c=malloc(sizeof(struct context));
	c->gd=malloc(sizeof(struct ext2_group_desc));

	if((fd_file=open(file,O_RDWR))==-1){
		//to do: init errno
		return NULL;
	}
	
	if(lseek(fd_file,1024,SEEK_SET)==-1){
		return NULL;
	}
	
	if((read(fd_file,&(c->sb),sizeof(struct ext2_super_block)))==-1){
		//to do: init errno
		return NULL;	
	}

	if(c->sb.s_magic!=EXT2_SUPER_MAGIC){
		//to do: init errno
		return NULL;
	}

	block_size=1024 << c->sb.s_log_block_size;
	nb_group=c->sb.s_blocks_count/c->sb.s_blocks_per_group + 1;

	c->fd=fd_file;
	c->ngroups=nb_group;

	lseek(fd_file,1024+block_size,SEEK_SET);
	
	if((read(fd_file,c->gd,sizeof(struct ext2_group_desc)))==-1){
		//to do: init errno
		return NULL;	
	}


	c->bufstat_read=0;
	c->bufstat_cached=0;
	c->last=NULL;

	while(i<maxbuf){
		if(c->last==NULL){
			c->last=malloc(sizeof(struct buffer));
			c->last->data=malloc(block_size);
			c->last->blkno=0;
			c->last->valid=0;
			c->last->next=c->last;
		}
		else{
			tmp=c->last->next;
			c->last->next=malloc(sizeof(struct buffer));
			c->last=c->last->next;
			c->last->data=malloc(block_size);
			c->last->blkno=0;
			c->last->valid=0;
			c->last->next=tmp;
		} 
		i++;
	}

	return c;
}

void e2_ctxt_close (ctxt_t c)
{
	buf_t tmp;
	buf_t tmp2;

	if(c!=NULL){
		close(c->fd);
		free(c->gd);

		tmp=c->last;
		while(tmp->next!=c->last){
			tmp2=tmp->next;
			free(tmp->data);
			free(tmp);
			tmp=tmp2;
		}
		free(tmp->data);
		free(tmp);
		free(c);
	}

}

int e2_ctxt_blksize (ctxt_t c)
{
	unsigned int block_size;
	
	if(c!=NULL){
		block_size=1024 << c->sb.s_log_block_size;
		return block_size;
	}
	else{
		return -1;
	}
}

/******************************************************************************
 * Fonctions de lecture non bufferisee d'un bloc
 */

int e2_block_fetch (ctxt_t c, pblk_t blkno, void *data)
{
	int i=e2_ctxt_blksize(c);
	lseek(c->fd,1024+i*blkno-1,SEEK_SET);
	
	if((read(c->fd,data,e2_ctxt_blksize(c)))==-1){
		//to do: init errno
		return 0;	
	}
	else{
		return 1;
	}
}

/******************************************************************************
 * Gestion du buffer et lecture bufferisee
 */

/* recupere un buffer pour le bloc, le retire de la liste des buffers
 * et lit le contenu si necessaire
 */
buf_t e2_buffer_get (ctxt_t c, pblk_t blkno)
{


	buf_t last_buffer=c->last;
	buf_t tmp;
	
	//cas relou a gerer
	if(last_buffer->blkno==blkno && last_buffer==last_buffer->next){
		c->bufstat_read++;
		c->last=NULL;
		return last_buffer;
	}
	else{
		while(last_buffer->next!=c->last){
			if(last_buffer->blkno!=blkno)
				last_buffer=last_buffer->next;
			else{
				c->bufstat_read++;
				tmp=last_buffer->next;
				last_buffer->next=last_buffer->next->next;
				return tmp;
			}
		}
	}
	
	if(c->last!=NULL){
		c->bufstat_cached++;
		void* data=malloc(e2_ctxt_blksize(c));
		e2_block_fetch(c,blkno,data);
		
		free(c->last->data);

		c->last->data=data;
		c->last->blkno=blkno;
		
		buf_t prev=c->last->next;
		tmp=c->last;
		
		while(prev->next!=c->last){
			prev=prev->next;
		}
		
		prev->next=c->last->next;
		c->last=prev;
		return tmp;
	}
	else
		return NULL;

}
        
/* replace le buffer en premier dans la liste */
void e2_buffer_put (ctxt_t c, buf_t b)
{
	buf_t tmp=c->last->next;

	c->last->next=b;
	b->next=tmp;

}
        
/* recupere les donnees du buffer */
void *e2_buffer_data (buf_t b)
{
	return b->data;
}

/* affiche les statistiques */
void e2_buffer_stats (ctxt_t c)
{
	printf("buffer reads : %d\n",c->bufstat_read);
	printf("buffe cached %d\n",c->bufstat_cached);
}

/******************************************************************************
 * Fonction de lecture d'un bloc dans un inode
 */

/* recupere le buffer contenant l'inode */
pblk_t e2_inode_to_pblk (ctxt_t c, inum_t i)
{
	int k;
	unsigned int blk_size=e2_ctxt_blksize(c);

	for(k=0;k<c->ngroups;k++){
			
		if(i<=c->sb.s_inodes_per_group*(k+1))
			return c->gd[k].bg_inode_table+(i-1)/(blk_size/sizeof(struct ext2_inode));
			
	}
	return -1;
}


/* extrait l'inode du buffer */
struct ext2_inode *e2_inode_read (ctxt_t c, inum_t i, buf_t b)
{
	i-=1;
	i%=e2_ctxt_blksize(c)/sizeof(struct ext2_inode);
	//copier la structure au cas ou le buf_t est detruit
	struct ext2_inode* inode_read=b->data+i*sizeof(struct ext2_inode);
	
	return inode_read;

}

/* numero de bloc physique correspondant au bloc logique blkno de l'inode in */
pblk_t e2_inode_lblk_to_pblk (ctxt_t c, struct ext2_inode *in, lblk_t blkno)
{


	int blksize = e2_ctxt_blksize(c);
	int nb_ind_per_block = blksize / (sizeof(int));
	int nb_ind_s = 13 + nb_ind_per_block;
	int nb_ind_d = nb_ind_s + nb_ind_per_block * nb_ind_per_block;
	int nb_ind_t = nb_ind_d + nb_ind_per_block * nb_ind_per_block * nb_ind_per_block;
	
	int numero_dans_blk_ind1;
	int numero_dans_blk_ind2;
	int numero_dans_blk_ind3;
	
	buf_t bloc_indirection1;
	buf_t bloc_indirection2;
	buf_t bloc_indirection3;
	
	int* blk_ind1;
	int* blk_ind2;
	int* blk_ind3;

	int pblk;

	if(blkno < 13){
		return in->i_block[blkno];
	}
	else if(blkno<nb_ind_s){
		
		bloc_indirection1=e2_buffer_get(c,in->i_block[13]);
		
		blk_ind1=(int*) bloc_indirection1->data;
		numero_dans_blk_ind1=blkno-13;

		pblk=blk_ind1[numero_dans_blk_ind1];
		
		e2_buffer_put(c,bloc_indirection1);
		return pblk;
		
	}
	else if(blkno < nb_ind_d){
		
		blkno-=13+nb_ind_s;
		
		bloc_indirection1=e2_buffer_get(c,in->i_block[14]);
		blk_ind1=(int*) bloc_indirection1->data;

		numero_dans_blk_ind1=blkno/nb_ind_per_block +1;
		numero_dans_blk_ind2=blkno%nb_ind_per_block;
		
		bloc_indirection2=e2_buffer_get(c,blk_ind1[numero_dans_blk_ind1]);
		pblk=((int*) bloc_indirection2->data)[numero_dans_blk_ind2];

		e2_buffer_put(c,bloc_indirection1);
		e2_buffer_put(c,bloc_indirection2);
		
		return pblk;

	}
	else if(blkno < nb_ind_t){
			
		blkno-=nb_ind_d;
			
		buf_t bloc_indirection1=e2_buffer_get(c,in->i_block[15]);

		blk_ind1=(int*) bloc_indirection1->data;

		numero_dans_blk_ind1= (blkno /(nb_ind_per_block*nb_ind_per_block));
		numero_dans_blk_ind2=(blkno%nb_ind_per_block*nb_ind_per_block)/nb_ind_per_block;
		numero_dans_blk_ind3=blkno%nb_ind_per_block;

	
		bloc_indirection2=e2_buffer_get(c,blk_ind1[numero_dans_blk_ind1]);
		blk_ind2=(int*) bloc_indirection2->data;
		bloc_indirection3=e2_buffer_get(c,blk_ind2[numero_dans_blk_ind2]);
		blk_ind3=(int*) bloc_indirection3->data;

		e2_buffer_put(c,bloc_indirection1);
		e2_buffer_put(c,bloc_indirection2);
		e2_buffer_put(c,bloc_indirection3);

		pblk=blk_ind3[numero_dans_blk_ind3];
		return pblk	;
	}

	return -1;
}

/******************************************************************************
 * Operation de haut niveau : affichage d'un fichier complet
 */

/* affiche les blocs d'un fichier */
int e2_cat (ctxt_t c, inum_t i, int disp_pblk)
{
}

/******************************************************************************
 * Simulation d'une ouverture de fichiers
 */

file_t e2_file_open (ctxt_t c, inum_t i)
{
}

void e2_file_close (file_t of)
{
}

/* renvoie EOF ou un caractere valide */
int e2_file_getc (file_t of)
{
}

/* renvoie nb de caracteres lus (0 lorsqu'on arrive en fin de fichier) */
int e2_file_read (file_t of, void *data, int len)
{
}

/******************************************************************************
 * Operations sur les repertoires
 */

/* retourne une entree de repertoire */
struct ext2_dir_entry_2 *e2_dir_get (file_t of)
{
}

/* recherche un composant de chemin dans un repertoire */
inum_t e2_dir_lookup (ctxt_t c, inum_t i, char *str, int len)
{
}

/******************************************************************************
 * Operation de haut niveau : affichage d'un repertoire complet
 */

/* affiche un repertoire donne par son inode */
int e2_ls (ctxt_t c, inum_t i)
{
}

/******************************************************************************
 * Traversee de repertoire
 */

/* recherche le fichier (l'inode) par son nom */
inum_t e2_namei (ctxt_t c, char *path)
{
}
