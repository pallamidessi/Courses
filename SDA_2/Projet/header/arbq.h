#ifndef _ARBQ_H__
#define _ARBQ_H__


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"couleur.h"

typedef int bool;

typedef struct Arbq{
	struct Arbq* no;
	struct Arbq* ne;
	struct Arbq* so;
	struct Arbq* se;
	couleur c;
}str_arbq,*arbq;

arbq f(couleur c);
arbq e(arbq a,arbq b,arbq c,arbq d);
arbq no(arbq a);
arbq ne(arbq a);
arbq so(arbq a);
arbq se(arbq a);
int hauteur(arbq a);
couleur c(arbq a);
bool estf(arbq a);
int nf(arbq a);
couleur p(int x,int y,arbq a);
void detruire_arbq(arbq a);

#endif
