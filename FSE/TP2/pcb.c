#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simul.h"

/*
 * Q1.2 : Creation d'un nouveau processus
 */


struct pcb *pcb_create (int pid, int start, int duration, int prio)
{
	struct pcb* new_pcb=malloc(sizeof(struct pcb));

	new_pcb->pid=pid;
	new_pcb->start_time=start;
	new_pcb->duration=duration;
	new_pcb->prio=prio;
	new_pcb->cpu_time=0;

	return new_pcb;
}

/*
 * Suppression d'un processus
 */

void pcb_destroy (struct pcb *pcb)
{
    free (pcb) ;
}
