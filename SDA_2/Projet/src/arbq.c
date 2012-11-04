#include"arbq.h"


arbq f(couleur c){
	
	arbq new_feuille=(arbq) malloc(sizeof(str_arbq));
	
	new_feuille->c=c;

	new_feuille->no=NULL;
	new_feuille->ne=NULL;
	new_feuille->so=NULL;
	new_feuille->se=NULL;

	return new_feuille;
}


arbq e(arbq a,arbq b,arbq c,arbq d){
	
	arbq new_racine=(arbq) malloc(sizeof(str_arbq));
	
	new_racine->c=NULL;

	new_feuille->no=a;
	new_feuille->ne=b;
	new_feuille->so=c;
	new_feuille->se=d;
	
	return new_racine;	
}


arbq no(arbq a){
	return a->no;	
}

arbq ne(arbq a){
	return a->ne;	
}

arbq so(arbq a){
	return a->so;	
}

arbq se(arbq a){
	return a->se;	
}

int hauteur(arbq a){
	
	if estf(a)
		return 1;
	else
		return hauteur(no(a))+1;
}

couleur c(arbq a){
	if(a->c!=NULL)
		return a->c;
}

bool v(arbq a){

	if(a==NULL)
		return 1;
	else 
		return 0;
}

bool estf(arbq a){
	if(no(a)==NULL)
		return 1;
	else
		return 0;
}

int nf(arbq a){
	if(estf(a))
		return 4;
	else 
		return nf(no(a))*4;
}

couleur p(int x,int y,arbq a){
	int moitie_taille=((int)sqrt(nf(a)))/2;

	if(estf(a))
		return c(a);
	else
		if(x>moitie_taille && y>moitie_taille)
			return p(x-moitie_taille,y-moitie_taille,se(a));
		else
			if(x>moitie_taille)
				return p(x-moitie_taille,y,ne(a));
			else
				if(y-moitie_taille)
					return p(x,y-moitie_taille,so(a))
				else
					p(x,y,no(a));
}
