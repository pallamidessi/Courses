#include"compteur.h"

void compteur(int decalY,int decalX){
	time_t start,end;
	double diff=0;
	time(&start);
	time(&end);
	diff=difftime(end,start);
	mvprintw(decalY+10,decalX+5,"%.2lf",diff);
	}
