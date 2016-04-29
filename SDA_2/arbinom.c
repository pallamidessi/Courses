#include"arbinom.h"

arbinom feuilleb(S x){
	arbinom a=(arbinom) malloc(sizeof(str_arbinom));
	enracinement(NULL,x,NULL);
	
	return a;
}

/* les fils sont a gauche et les freres a droite*/
arbinom lien(arbinom a1,arbinom a2){
	(extre_droite(a1->b->ag))->ag=a2; /*l'arbre a lie est un fils de la racine de a1,on
	parcours ses freres(extre_droite) et on l'accroche aux dernier freres*/

free(a2->b);
free(a2);
return a1;

}


S racineb(arbinom a){
	return racine(a->b);	
}


bool estFeuille(arbinom a){
	return feuille(a->b);		
}


arbinom premierFils(arbinom a){
	return arbre_gauche(a->b);
}


arbinom autreFils(arbinom a){
	return arbre_droit(premierFils(a));
	
}

int hauteurb(arbinom a){
	return hauteur(a->b);
}
	
int nbr_noeudb(arbinom a){
	return nbr_noeud(a->b); 
}

int nbr_noeudInterneb(arbinom a){
	return nbr_noeudInterne(a->b);
}

int nbr_feuilleb(arbinom a){
	return nbr_feuille(a->b);
}

int nbr_noeuds_hauteur(arbinom a,int h){
	if(hauteurb(a)==h)
		return 1;
	else 
		return nbr_noeuds_hauteur(premierFils(a),h)+nbr_noeuds_hauteur(autreFils(a),h);
}
	


