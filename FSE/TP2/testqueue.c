#include"simul.h"
#include<stdio.h>
#include<stdlib.h>


int main(){
	
	struct pcb* new1=pcb_create(2,1,8,7);
	struct pcb* new2=pcb_create(5,2,32,7);
	struct pcb* new3=pcb_create(15,10,45,7);
	struct pcb* new4=pcb_create(28,78,2,15);
	struct pcb* new5=pcb_create(7,89,45,74);
	struct pcb* new6=pcb_create(9,47,96,16);


	queue_init();
	queue_insert(new1);
	queue_insert(new2);
	queue_insert(new3);
	queue_insert(new4);
	queue_insert(new5);
	queue_insert(new6);


	queue_display();
	
	
	
	
	return 0;
}
