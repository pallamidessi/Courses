#include<stdio.h>
#include<stdlib.h>

#define true 1
#define	false 0
typedef int bool;

typedef struct Rel{
int taille;
int* tab;
}str_rel,*rel;

rel rv();
rel rp(int n);
int tr(rel r);
rel aa(rel r,int i,int j);
bool ea(rel r,int i,int j);
rel sa(rel r,int i,int j);
rel ru(rel r1,rel r2);
rel ri(rel r1,rel r2);
rel rs(rel r);
rel rfr(rel r);
rel rfs(rel r);
rel rft(rel r);
void pr(rel r);



