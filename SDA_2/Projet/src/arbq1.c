/**
 * \file			arbq1.c
 * \author		Pallamidessi Joseph
 * \version		1.0
 * \date			02/12/2012
 * \brief			src arbq1.c
 *
 * \details		Fonctions associees a arbq1.h
*/
#include"arbq1.h"

Arbq damier(int n){
	int hauteur_max;
	int test=1;

	if(n%2!=0){
		printf("la taille specifie n'est pas un multiple de 2\n");
		return NULL;
	}
	
	while(n*n!=test){
		test*=4;
		hauteur_max++;
	}

	Arbq dam=f(NULL);

	return damier1(dam,0,hauteur_max);
}

Arbq damier1(Arbq a,int hauteur_act,int hauteur_max){
	
	if(hauteur_max==hauteur_act)
		return e(f(blanc()),f(noir()),f(noir()),f(blanc()));
	else
		a->no=f(NULL);
		a->ne=f(NULL);
		a->so=f(NULL);
		a->se=f(NULL);
		return e(damier1(no(a),hauteur_act+1,hauteur_max),damier1(ne(a),hauteur_act+1,hauteur_max),
							damier1(so(a),hauteur_act+1,hauteur_max),damier1(se(a),hauteur_act+1,hauteur_max));
}

Arbq symh1(Arbq a){
	if(estf(no(a)))
		return e(so(a),se(a),no(a),ne(a));
	else
		return e(symh1(so(a)),symh1(se(a)),symh1(no(a)),symh1(ne(a)));
	}

Arbq symh(Arbq a){
	
		return e(no(a),ne(a),symh1(no(a)),symh1(ne(a)));
}

Arbq symv1(Arbq a){
	if(estf(no(a)))
		return e(ne(a),no(a),se(a),so(a));
	else
		return e(symv1(ne(a)),symv1(no(a)),symv1(se(a)),symv1(so(a)));
	}

Arbq symv(Arbq a){
	
		return e(no(a),symv1(no(a)),so(a),symv1(so(a)));
}

Arbq rotg(Arbq a){
	if(estf(no(a)))
		return e(ne(a),se(a),no(a),so(a));
	else
		return e(rotg(ne(a)),rotg(se(a)),rotg(no(a)),rotg(so(a)));
	
}

Arbq rotd(Arbq a){
	if(estf(no(a)))
		return e(so(a),no(a),se(a),ne(a));
	else
		return e(rotd(so(a)),rotd(no(a)),rotd(se(a)),rotd(ne(a)));
	
}

Arbq dzoo(Arbq a){
	if(estf(no(no(a))))
		return e(no(no(a)),no(ne(a)),no(so(a)),no(se(a)));
	else 
		return e(dzoo(no(a)),dzoo(ne(a)),dzoo(so(a)),dzoo(se(a)));
}


Arbq parc(Arbq a,Arbq(*operation)(Arbq)){

	if(estf(no(a)))
		return e(operation(no(a)),operation(ne(a)),operation(so(a)),operation(se(a)));
	else
		return e(parc(no(a),(*operation)),parc(ne(a),(*operation)),
		parc(so(a),(*operation)),parc(se(a),(*operation)));

}

Arbq parc1(Arbq a,Arbq(*operation)(Arbq,int),int seuil){

	if(estf(no(a)))
		return
		e(operation(no(a),seuil),operation(ne(a),seuil),operation(so(a),seuil),operation(se(a),seuil));
	else
		return e(parc1(no(a),(*operation),seuil),parc1(ne(a),(*operation),seuil),
					   parc1(so(a),(*operation),seuil),parc1(se(a),(*operation),seuil));

}

Arbq invc(Arbq a){
	return f(ic(255-r(c(a)),255-v(c(a)),255-b(c(a))));		
}

Arbq nivg(Arbq a){
	
	unsigned char niv_gris=(unsigned char)(r(c(a))*0.299+v(c(a))*0.578+b(c(a))*0.114);
	
	return f(ic(niv_gris,niv_gris,niv_gris));
}

Arbq tresh(Arbq a,int seuil){
	
	unsigned char niv_gris=(unsigned char)(r(c(a))*0.299+v(c(a))*0.578+b(c(a))*0.114);
	
	if(niv_gris<seuil)
		return f(ic(r(c(a))+50,v(c(a))+50,b(c(a))+50));
	else
		return a;
}
