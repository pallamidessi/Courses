#include "include.h"

Matrix * addition(Matrix_couple* m) { 
  Matrix a,b;
  static Matrix c;
  int i,j;
  static int dim;

  if (c!= NULL) {
    for (i = 0; i < dim; i++) {
      xdr_freep(c->mat[i]);
    }
    xdr_freep(c);
  }

  dim=(*m)->dim;

  c=new_matrix(dim); //to free
  a=(*m)->a;
  b=(*m)->b;

  print_matrix_couple(*m);

  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      c->mat[i][j]=a->mat[i][j]+b->mat[i][j];
    }
  }
  Matrix* res=&c;

  return res;
}

Matrix * multiplication(Matrix_couple *m) { 
  Matrix a,b;
  static Matrix c;
  int i,j,k;
  static int dim;

  if (c!= NULL) {
    for (i = 0; i < dim; i++) {
      xdr_freep(c->mat[i]);
    }
    xdr_freep(c);
  }

  dim=(*m)->dim;

  c=new_matrix(dim); //to free
  a=(*m)->a;
  b=(*m)->b;

  print_matrix_couple(*m);
  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      for (k = 0; k < dim; k++) {
        c->mat[i][j]+=a->mat[i][k]*b->mat[k][i];
      }
    }
  }
  Matrix* res=&c;

  return res;
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


