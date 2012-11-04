#include"arbq1.h"

/*
arbq damier(int n){
	
	arbq a=f(NULL);

	while(nf(a)<n*n)

	
}
*/

arbq symh(arbq a){
	
	if(estf(no(a)))
		return e(se(a),so(a),no(a),ne(a));
	else
		return e(no(a),ne(a),symh(no(a)),symh(ne(a)));
}

arbq symv(arbq a){
	
	if(estf(no(a)))
		return e(se(a),so(a),no(a),ne(a));
	else
		return e(no(a),ne(a),symv(no(a)),symv(ne(a)));
}

arbq rotg(arbq a){
	if(estf(no(a)))
		return e(ne(a),se(a),no(a),so(a));
	else
		return e(rotg(ne(a)),rotg(se(a)),rotg(no(a)),rotg(so(a)));
	
}

arbq rotd(arbq a){
	if(estf(no(a)))
		return e(so(a),no(a),se(a),ne(a));
	else
		return e(rotd(so(a)),rotd(no(a)),rotd(se(a)),rotd(ne(a)));
	
}

arbq dzoo(arbq a){
	if(estf(no(no(a))))
		return e(no(no(a)),no(ne(a)),no(so(a)),no(se(a)));
	else 
		return e(dzoo(no(a)),dzoo(ne(a)),dzoo(so(a)),dzoo(se(a)));
}


arbq parc(arbq a,arbq(*operation)(arbq)){

	if(estf(no(a)))
		return e(operation(no(a)),operation(ne(a)),operation(so(a)),operation(se(a)));
	else
		return e(parc(no(a),(*operation)),parc(ne(a),(*operation)),
		parc(so(a),(*operation)),parc(se(a),(*operation)));

}

arbq invc(arbq a){
	return f(ic(255-r(c(a)),255-v(c(a)),255-b(c(a))));		
}

arbq nivg(arbq a){
	
	unsigned char niv_gris=(unsigned char)(r(c(a))*0,299+v(c(a))*0,578+b(c(a))*0,114);
	
	return f(ic(niv_gris,niv_gris,niv_gris));
}

arbq tresh(arbq a,int seuil){
	
	unsigned char niv_gris=(unsigned char)(r(c(a))*0,299+v(c(a))*0,578+b(c(a))*0,114);
	
	if(niv_gris<seuil)
		return f(ic(r(c(a))+50,v(c(a))+50,b(c(a))+50));
	else
		return a;
}
