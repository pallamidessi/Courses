#ifndef __ARBINT_H
#define __ARBINT_H

#include<stdio.h>
#include<stdlib.h>

#define S int
#define bool int

typedef struct arbint{
struct arbin *ag;	
struct arbin *ad;	
S etiquette;
}str_arbint,*arbint;

arbint lambda();
arbint e(arbin gauche,S x,arbin droit);
arbint ag(arbin a);
arbint ad(arbin a);
S r(arbin a);
bool v(arbin a);
S min(arbin a);
S max(arbin a);
arbint rech(arbin a,S x);
arbint otermin(arbin a);
arbint otermax(arbin a);
arbint sup(arbin a);

