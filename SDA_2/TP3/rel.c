#include"rel.h"


rel rv(int n){
	rel new=(rel)malloc(sizeof(str_rel));
	
	new->tab=(int*)calloc(n*n,sizeof(int));
	new->taille=n;

	return new;
}
	
rel rp(int n){
	int i,j;

	rel new=(rel)malloc(sizeof(str_rel));
	
	new->tab=(int*)malloc(n*sizeof(int));
	new->taille=n;
	
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			new->tab[i*n+j]=1;
		}
	}

	return new;
	
}

int tr(rel r){
	return r->taille;
}

rel aa(rel r,int i,int j){
	r->tab[i*r->taille+j]=1;
	return r;
}

bool ea(rel r,int i,int j){
	
	if(r->tab[i*r->taille+j]==1)
		return true;
	else 
		return false;
}

rel sa(rel r,int i,int j){
	r->tab[i*r->taille+j]=0;
	return r;
}

rel ru(rel r1,rel r2){
	int i,j;
	int taille=r1->taille;
	
	for(i=0;i<taille;i++){
		for(j=0;j<taille;j++){
			if(r2->tab[i*taille+j]==1)
				r1->tab[i*taille+j]=1;
		}
	}
return r1;
}

rel ri(rel r1,rel r2){
	int i,j;
	int taille=r1->taille;
	
	for(i=0;i<taille;i++){
		for(j=0;j<taille;j++){
			if(r2->tab[i*taille+j]==1 && r1->tab[i*taille+j]==1)
				r1->tab[i*taille+j]=1;
			else
				r1->tab[i*taille+j]=0;
		}
	}
	return r1;
}

rel rfs(rel r){
	int i,j;
	int taille=r->taille;

	for(i=0;i<taille;i++){
		for(j=0;j<taille;j++){
			if(r->tab[i*taille+j]==1)
				r->tab[taille*j+i]=1;
		}
	}

return r;
}


rel rfr(rel r){
	int taille=r->taille;
	int i;

	for(i=0;i<taille;i++)
		r->tab[i*taille+i]=1;

	return r;
}


rel rs(rel r){
	int i,j,tmp;
	int taille=r->taille;
	
	for(i=0;i<taille;i++){
		for(j=0;j<taille;j++){
			tmp=r->tab[i*j+j];
				r->tab[i*taille+j]=r->tab[j*taille+i];
				r->tab[i*taille+i]=tmp;
		}
	}
	return r;	
}

/*
rel rft(rel r){
}
*/

void pr(rel r){
	int i,j;
	int taille=r->taille;
	
	for(i=0;i<taille;i++){
		for(j=0;j<taille;j++){
				printf("%d ",r->tab[i*taille+j]);
		}
	printf("\n");
	}
}


