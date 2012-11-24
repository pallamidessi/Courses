#include"arbq.h"


Arbq f(Couleur c){
	
	Arbq new_feuille=(Arbq) malloc(sizeof(str_arbq));
	
	new_feuille->c=c;

	new_feuille->no=NULL;
	new_feuille->ne=NULL;
	new_feuille->so=NULL;
	new_feuille->se=NULL;

	return new_feuille;
}


Arbq e(Arbq a,Arbq b,Arbq c,Arbq d){
	
	Arbq new_racine=(Arbq) malloc(sizeof(str_arbq));
	
	new_racine->c=NULL;

	new_racine->no=a;
	new_racine->ne=b;
	new_racine->so=c;
	new_racine->se=d;
	
	return new_racine;	
}


Arbq no(Arbq a){
	return a->no;	
}

Arbq ne(Arbq a){
	return a->ne;	
}

Arbq so(Arbq a){
	return a->so;	
}

Arbq se(Arbq a){
	return a->se;	
}

int hauteur(Arbq a){
	
	if (estf(a))
		return 1;
	else
		return hauteur(no(a))+1;
}

Couleur c(Arbq a){
		return a->c;
}


bool estf(Arbq a){
	if(no(a)==NULL)
		return 1;
	else
		return 0;
}

int nf(Arbq a){
	if(estf(a))
		return 1;
	else 
		return 4*nf(no(a));
}

Arbq p(Arbq a,int x,int y){
	int moitie_taille=((int)sqrt(nf(a)))/2;

	if(estf(a))
		return a;
	else
		if(x>moitie_taille && y>moitie_taille)
			return p(se(a),x-moitie_taille,y-moitie_taille);
		else
			if(x>moitie_taille)
				return p(so(a),x-moitie_taille,y);
			else
				if(y>moitie_taille)
					return p(ne(a),x,y-moitie_taille);
				else
					return p(no(a),x,y);
}
