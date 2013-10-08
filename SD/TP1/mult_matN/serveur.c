#include "include.h"

matrix * addition(matrix_couple *m) { 
  Matrix c,a,b;
	int i,j;
	int dim=m->dim;

	c=new_matrix(dim);
	a=m->a;
	b=m->b;

	print_matrix_couple(m);

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			c->mat[i][j]=a->mat[i][j]+b->mat[i][j];
		}
	}
	return c;
}
matrix * multiplication(matrix_couple *m) { 
  Matrix c,a,b;
	int i,j,k;
	int dim=m->dim;
	
	c=new_matrix(dim);
	a=m->a;
	b=m->b;

	print_matrix_couple(m);
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			for (k = 0; k < dim; k++) {
				c->mat[i][j]+=a->mat[i][k]*b->mat[k][i];
			}
		}
	}
	return c;
}

int main (void) {
  bool_t stat;
 
  stat = registerrpc(/* prognum */ PROGNUM,
		     /* versnum */ VERSNUM,
		     /* procnum */ PROCNUM_MULT,
		     /* pointeur sur fonction */  multiplication,
		     /* decodage arguments */ (xdrproc_t)xdr_matrix_couple,
		     /* encodage retour de fonction */ (xdrproc_t)xdr_matrix);
  
	stat = registerrpc(/* prognum */ PROGNUM,
		     /* versnum */ VERSNUM,
		     /* procnum */ PROCNUM_ADD,
		     /* pointeur sur fonction */  addition,
		     /* decodage arguments */ (xdrproc_t)xdr_matrix_couple,
		     /* encodage retour de fonction */ (xdrproc_t)xdr_matrix);

  if (stat != 0) {
    fprintf(stderr,"Echec de l'enregistrement\n");
    exit(1);
  }
  svc_run(); /* le serveur est en attente de clients eventuels */
  return(0); /* on y passe jamais ! */
}


