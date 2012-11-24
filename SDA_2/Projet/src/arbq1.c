#include"arbq1.h"

/*
Arbq damier(int n){
	
	Arbq a=f(NULL);

	while(nf(a)<n*n)

	
}
*/
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
