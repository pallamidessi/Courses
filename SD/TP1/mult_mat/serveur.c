#include "include.h"

matrix * addition(Matrix_couple m) { 
  static matrix c;
	int i,j;
	

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			c.mat[i][j]=m->a.mat[i][j]+m->b.mat[i][j];
		}
	}
	return &c;
}

matrix * multiplication(Matrix_couple m) { 
  static matrix c;
	int i,j,k;
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			for (k = 0; k < 2; k++) {
				c.mat[i][j]+=m->a.mat[i][k]*m->b.mat[k][i];
			}
		}
	}
	return &c;
}

int main (void) {
  bool_t stat;
  bool_t stat2;
 
  stat = registerrpc(/* prognum */ PROGNUM,
		     /* versnum */ VERSNUM,
		     /* procnum */ PROCNUM_MULT,
		     /* pointeur sur fonction */  multiplication,
		     /* decodage arguments */ (xdrproc_t)xdr_matrix_couple,
		     /* encodage retour de fonction */ (xdrproc_t)xdr_matrix);
  
	stat2 = registerrpc(/* prognum */ PROGNUM,
		     /* versnum */ VERSNUM,
		     /* procnum */ PROCNUM_ADD,
		     /* pointeur sur fonction */  addition,
		     /* decodage arguments */ (xdrproc_t)xdr_matrix_couple,
		     /* encodage retour de fonction */ (xdrproc_t)xdr_matrix);

  if (stat != 0 && stat2 != 0) {
    fprintf(stderr,"Echec de l'enregistrement\n");
    exit(1);
  }

  svc_run(); /* le serveur est en attente de clients eventuels */
  
  return(0); /* on y passe jamais ! */
}


