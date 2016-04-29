#include "include.h"

matrix * addition(matrix_couple *m) { 
  static matrix c;
  int i,j;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      c[i][j]=m->a[i][j]+m->b[i][j];
    }
  }
  return &c;
}
matrix * multiplication(matrix_couple *m) { 
  static matrix c;
  int i,j;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      c[i][j]+=m->a[i][j]*m->b[j][i];
    }
  }
  return &c;
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


