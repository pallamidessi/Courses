#include <stdio.h>
#include <stdlib.h>

#include "simul.h"

/******************************************************************************
 * Structures de donnees
 *
 * Cette structure de donnees est opaque : elle n'est pas connue
 * a l'exterieur de ce fichier source.
 */

/* Cellule d'une file de processus */
struct cell
{
    struct pcb *proc ;
    struct cell *next ;
} ;

/* La file de processus pour l'ordonnanceur round-robin */
struct cell *rrqueue ;

/******************************************************************************
 * Operations generiques de l'ordonnanceur
 */

/*
 * Initialisation de la structure de donnees
 */

void queue_init (void)
{
    rrqueue = NULL ;
}

/*
 * Premier processus de la file
 */

struct pcb *queue_head (void)
{
    struct pcb *p ;

    if (rrqueue != NULL)
	p = rrqueue->next->proc ;
    else p = NULL ;

    return p ;
}

/*
 * Suppression du premier processus de la file
 * On suppose que le pcb existe forcement
 */

void queue_remove_first (struct pcb *p)
{
    if (rrqueue != NULL)
    {
	struct cell *c ;

	c = rrqueue->next ;
	if (rrqueue->next == rrqueue)
	    rrqueue = NULL ;
	else
	    rrqueue->next = rrqueue->next->next ;
	free (c) ;
    }
    else fprintf (stderr, "tentative de vider une file vide");
}

/*
 * Q1.4 : Suppression d'un processus quelconque de la file
 * On suppose que le pcb existe forcement
 */

void queue_remove_any (struct pcb *p)
{
    struct cell* tmp;
    struct cell* tail=rrqueue;
		
		if(rrqueue==NULL)
			return;

    if(rrqueue->next==rrqueue){
			if(rrqueue->proc==p){
				free(rrqueue->proc);
				free(rrqueue);
			}
			return;
		}
		
		while(rrqueue->next!=tail){
			if(rrqueue->next->proc!=p)
				rrqueue=rrqueue->next;
			else{
				tmp=rrqueue->next->next;
				free(rrqueue->next->proc);
				free(rrqueue->next);
				rrqueue->next=tmp;
				break;
			}
		}
		rrqueue=tail;
}

/*
 * Q1.3 : Insertion du processus en fin de file
 */

void queue_insert (struct pcb *p)
{
    struct cell* tmp;	
		struct cell* new=malloc(sizeof(struct cell));
		
		new->proc=p;
		new->next;
		
		if(rrqueue==NULL)
			rrqueue=new;
			new->next=new;
		else{
			tmp=rrqueue->next;
			rrqueue->next=new;
			new->next=tmp;
		}
}

/*
 * Q1.5 : Affichage de la file
 */

void pcb_display(struct pcb* p){
	
	printf("pid : %d\n",p->pid);
	printf("start:%d\n",p->start_time);
	printf("duration :%d\n",p->duration);
	printf("prio %d\n",p->prio);
	printf("cpu time: %d\n",p->cpu_time);
}



void queue_display (void)
{
    struct cell* tail=rrqueue;
		
		if(rrqueue==NULL)
			return;
		else 
			if(rrqueue->next==rrqueue){
				pcb_display(rrqueue->proc);
			}
		else{
			pcb_display(rrqueue->proc);
			while(rrqueue->next!=tail){
				pcb_display(rrqueue->next->proc);
				rrqueue=rrqueue->next;
			}
		}
}

/*
 * Q2.2 : Ordonnanceur selon l'algorithme du tourniquet
 */

struct pcb *schedule (struct pcb *curproc)
{
    /* A ecrire */
}
