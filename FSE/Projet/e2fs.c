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
	
	/* Initialisation du super block */

	if((fd_file=open(file,O_RDWR))==-1){
		return NULL;
	}
	
	if(lseek(fd_file,1024,SEEK_SET)==-1){
		return NULL;
	}
	
	if((read(fd_file,&(c->sb),sizeof(struct ext2_super_block)))==-1){
		return NULL;	
	}

	if(c->sb.s_magic!=EXT2_SUPER_MAGIC){
		errno=ENOTSUP;
		return NULL;
	}

	block_size=1024 << c->sb.s_log_block_size;
	nb_group=c->sb.s_blocks_count/c->sb.s_blocks_per_group + 1;

	c->fd=fd_file;
	c->ngroups=nb_group;

	lseek(fd_file,1024+block_size,SEEK_SET);
	
	if((read(fd_file,c->gd,sizeof(struct ext2_group_desc)))==-1){
		return NULL;	
	}


	c->bufstat_read=0;
	c->bufstat_cached=0;
	c->last=NULL;

	/* Initialisation des buffers */
	
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

		/* Free des elements de la liste chainee circulaire de buffer */
		
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
	lseek(c->fd,1024+i*(blkno-1),SEEK_SET);
	
	if((read(c->fd,data,e2_ctxt_blksize(c)))==-1){
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
	/* Recherche si le buffer est deja dans la liste, et le renvoi si oui. */
	
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

	/*Le buffer n'as pas ete trouve dans la liste, on en prend un libre si possible pour
	 * l'initialiser et on le renvoi. */

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
		errno=ENOBUFS;
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
	
	/* On devrait copier la structure au cas ou le buf_t est detruit */
	
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

	/* Pas d'indirection */
	if(blkno < 13){
		
		if (in->i_block[blkno]==0) {
			return -1;
		}

		return in->i_block[blkno];

	}
	/* Premier bloc d'indirection */
	else if(blkno<nb_ind_s){
		
		bloc_indirection1=e2_buffer_get(c,in->i_block[13]);
		
		blk_ind1=(int*) bloc_indirection1->data;
		numero_dans_blk_ind1=blkno-13;

		pblk=blk_ind1[numero_dans_blk_ind1];
		
		if (pblk==0) {
			return -1;
		}

		e2_buffer_put(c,bloc_indirection1);
		return pblk;
		
	}
	/* Deuxieme bloc d'indirection */
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
		
		if (pblk==0) {
			return -1;
		}

		return pblk;

	}
	/* Troisieme bloc d'indirection */
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
		
		if (pblk==0) {
			return -1;
		}

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
	int pblk=e2_inode_to_pblk(c,i);
	buf_t buf=e2_buffer_get(c,pblk);
	struct ext2_inode* in=e2_inode_read(c,i,buf);
	int j=0;	
	pblk_t current_blk;
	buf_t current_buf;
	
	if(disp_pblk==0){
		
		while(in->i_block[j]!=0){
			current_blk=e2_inode_lblk_to_pblk(c,in,j);
			current_buf=e2_buffer_get(c,current_blk);

			write(1,current_buf->data,e2_ctxt_blksize(c));
			printf("\n");

			e2_buffer_put(c,current_buf);
			j++;
		}

	}
	else{
		printf("taille en octets:%d\n",in->i_size);
		while(in->i_block[j]!=0){
			printf("bloc physique %d\n",in->i_block[j]);
			j++;
		}
	}

	e2_buffer_put(c,buf);	
	return 0;
}

/******************************************************************************
 * Simulation d'une ouverture de fichiers
 */

file_t e2_file_open (ctxt_t c, inum_t i)
{
	file_t f=malloc(sizeof(struct ofile));
	f->ctxt=c ;																									/* eviter param a chaque e2_file_xxx */
	pblk_t pb=e2_inode_to_pblk(c,i);
	f->buffer=e2_buffer_get(c,pb);
	
	f->inode=e2_inode_read(c,i,f->buffer);											/* l'inode proprement dit */
	f->curblk=0 ;																								/* position en bloc */
	f->data =malloc(e2_ctxt_blksize(c));												/* donnees */
	f->len=e2_ctxt_blksize(c);																	/* longueur utile dans les donnees */
	f->pos=0;
	return f;
}

void e2_file_close (file_t of)
{
	e2_buffer_put(of->ctxt,of->buffer);
	free(of->data);
	free(of);
}

/* renvoie EOF ou un caractere valide */
int e2_file_getc (file_t of)
{
	buf_t p_data;
	ctxt_t ctxt=of->ctxt;
	int blksize=e2_ctxt_blksize(ctxt);
	int curblk=of->curblk;
	int len=of->len;
	int pos=of->pos;
	void* data=of->data;
	pblk_t next_blk;
	
	
	if(curblk==0){																													/* Premier appel a getc, on charge uniquement le premier bloc dans data */
		p_data=e2_buffer_get(ctxt,e2_inode_lblk_to_pblk(ctxt,of->inode,0));
		memcpy(data,e2_buffer_data(p_data),blksize);
		e2_buffer_put(ctxt,p_data);

		of->curblk=1;
    of->len=blksize;		
	}
	else if(len==pos){
		if((next_blk=e2_inode_lblk_to_pblk(ctxt,of->inode,curblk+1))!=-1){		/* Cas changement de bloc */
			p_data=e2_buffer_get(ctxt,next_blk);
			memcpy(data,e2_buffer_data(p_data),blksize);
			e2_buffer_put(ctxt,p_data);
			
			of->curblk+=1;
    	of->len=blksize;
			of->pos=0;
		}
		else 
			return -1;
	}
	else{
		of->pos++;																														/* Retour du caractere courant */
		return (int) (((char*) of->data)[pos]);
	}
	return 0;
}

/* renvoie nb de caracteres lus (0 lorsqu'on arrive en fin de fichier) */
int e2_file_read (file_t of, void *data, int len)
{
	int i;
	int last_char;
	for(i=0;i<len;i++){
		if((last_char=e2_file_getc(of))!=EOF)
			((char*) data)[i]=(char) last_char;
		else
			return 0;
	}
	return i;
}

/******************************************************************************
 * Operations sur les repertoires
 */

/* retourne une entree de repertoire */
struct ext2_dir_entry_2 *e2_dir_get (file_t of)
{
	static struct ext2_dir_entry_2 entry;
	
	int pos=of->pos;

	if (pos==0 && of->curblk==0) {
		e2_file_getc(of);
	}
	

	if(S_ISDIR(of->inode->i_mode)){

		memcpy(&entry,((char*)of->data)+of->pos,sizeof(struct ext2_dir_entry_2));
		
		entry.name[entry.name_len+1]='\0';
		
		/* read "dans le vent" pour deplacer le curseur virtuel au debut de la prochaine
		 * entree,possiblement dans le bloc suivant*/

		char buf[sizeof(char)*entry.rec_len+1];
		if(e2_file_read(of,buf,entry.rec_len)==0)
			return NULL;
	}
	else{
		printf("pas un dossier\n");
		return NULL;
	}


	return &entry;
}

/* recherche un composant de chemin dans un repertoire */
inum_t e2_dir_lookup (ctxt_t c, inum_t i, char *str, int len)
{
	file_t of=e2_file_open(c,i);
	struct ext2_dir_entry_2* entry;

	while((entry=e2_dir_get(of))!=NULL){
		if(strncmp(str,entry->name,len)==0){
			e2_file_close(of);
			return entry->inode;
		}
	}
	
	e2_file_close(of);
	errno=ENOENT;
	
	return 0;
}

/******************************************************************************
 * Operation de haut niveau : affichage d'un repertoire complet
 */

/* affiche un repertoire donne par son inode */
int e2_ls (ctxt_t c, inum_t i)
{
	file_t of=e2_file_open(c,i);
	struct ext2_dir_entry_2* entry;

	while((entry=e2_dir_get(of))!=NULL){
		printf("%d %s\n",entry->inode,entry->name);
	}
	
	e2_file_close(of);
	return 0;
}

/******************************************************************************
 * Traversee de repertoire
 */

/* recherche le fichier (l'inode) par son nom */
inum_t e2_namei (ctxt_t c, char *path)
{
	int i=0;
	int sz_parsed_path[512];
	int inode_to_search=2;
	int nb_parsed_path=0;
	char** parsed_path=calloc(512,sizeof(char*));
	char* pp=path;

	for (i = 0; i < 512; i++) {
		sz_parsed_path[i]=0;
	}
	i=0;
	
	while((pp=strchr(pp,'/'))!=NULL){
		parsed_path[i]=pp+1;
		pp++;
		i++;
	}

	if (parsed_path[0]==NULL) {
		parsed_path[0]=path;
	}

	while (parsed_path[nb_parsed_path]!=NULL) {
		nb_parsed_path++;
	}

	i=0;
	if (nb_parsed_path==1) {
		if(parsed_path[0][0]=='/'){
			printf("test\n");
			parsed_path[0]=path+1;
		}
		sz_parsed_path[0]=strlen(parsed_path[0]);
	}
	else{
		while(parsed_path[i+1]!=NULL){
			sz_parsed_path[i]=parsed_path[i+1]-parsed_path[i]-1;
			i++;
		}
		sz_parsed_path[i]=strlen(parsed_path[i]);
	}
		
	for (i = 0; i < nb_parsed_path; i++) {
		
		if ((inode_to_search=e2_dir_lookup(c,inode_to_search,parsed_path[i],sz_parsed_path[i]))!=0){
			continue;
		}
		else {
			free(parsed_path);
			return 0;
		}
	}

	free(parsed_path);
	
	return inode_to_search;
}
