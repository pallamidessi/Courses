#include"arbin2.h"


arbin arbre_nouv(){
	return NULL;
}


arbin enracinement(arbin gauche,S x,arbin droit){
	arbin r=(arbin) malloc(sizeof(str_arbin));

	r->ag=gauche;
	r->ad=droit;
	r->etiquette=x;

	return r;
}


arbin arbre_gauche(arbin a){
	return a->ag;
}


arbin arbre_droit(arbin a){
	return a->ad;
}


S racine(arbin a){
	return a->etiquette;
}


bool vide(arbin a){
	return (a==NULL);
}


int max(int a,int b){
	return ((a>b) ?  a : b);
}
	
	
int hauteur(arbin a){
	if(vide(a))
		return 0;
	else 
		return max(hauteur(a->ag),hauteur(a->ad)) + 1;
}


arbin extre_gauche(arbin a){
	if(vide(a->ag))
		return a;
	else 
		return extre_gauche(a->ag);
}


arbin extre_droite(arbin a){
	if(vide(a->ad))
		return a;
	else 
		return extre_droite(a->ad);
}


bool feuille(arbin a){
	return (vide(a->ag)&&vide(a->ad));	
}


int nbr_feuille(arbin a){
	if(feuille(a))
		return 1;
	else return nbr_feuille(a->ag)+nbr_feuille(a->ad);
}


int nbr_noeud(arbin a){
	if(vide(a))
		return 0;
	else 
		return nbr_noeud(a->ad)+nbr_noeud(a->ag)+1;
}


int nbr_noeudInterne(arbin a){
	if(feuille(a))
		return 0;
	else 
		return nbr_noeudInterne(a->ag)+ nbr_noeudInterne(a->ag)+1;
}		

bool ega_arbre(arbin a,arbin b){
	return (a->etiquette==b->etiquette)&&
					ega_arbre(a->ag,b->ag)&&
					ega_arbre(a->ad,b->ad);
}

	
void affiche_terme(arbin a){
	
	if(vide(a))
		return;	
	else{
		printf("(");
		affiche_terme(a->ag);
		printf("%s",racine(a));
		affiche_terme(a->ad);
		printf(")");
	}
}
	
int evaluer(arbin a){
	
	if (feuille(a)){
		if(racine(a)[0]==('+'||'-'||'*'||'/')){
			printf("erreur operande attendu");
			exit(2);
		}
		else
			return atoi(racine(a));
	}
	else{
		if(racine(a)[0]=='+')
			return evaluer(a->ag)+evaluer(a->ad);
		else 
			if(racine(a)[0]=='*')
				return evaluer(a->ag)*evaluer(a->ad);
		else 
			if(racine(a)[0]=='-')
				return evaluer(a->ag)-evaluer(a->ad);
		else{ 
			if(racine(a)[0]=='/'){
				if (evaluer(a->ad)!=0)
					return evaluer(a->ag)/evaluer(a->ad);
				else {
				printf("division par 0: arret");
				exit(1);
				}
			}
		}
	}
}
