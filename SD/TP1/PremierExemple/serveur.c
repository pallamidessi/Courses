#include "include.h"

int * proc_dist(int *n) { 
  static int res = 1;
  printf("serveur: variable n (debut) : %d,\n",*n);
  res = (*n) + 1;
  *n = *n + 1;
  printf("serveur: variable n (fin) : %d,\n",*n);
  printf("serveur: variable res : %d\n",res);
  return &res;
}


int main (void) {
  bool_t stat;
  stat = registerrpc(/* prognum */ PROGNUM,
		     /* versnum */ VERSNUM,
		     /* procnum */ PROCNUM,
		     /* pointeur sur fonction */  proc_dist,
		     /* decodage arguments */ (xdrproc_t)xdr_int,
		     /* encodage retour de fonction */ (xdrproc_t)xdr_int);
  if (stat != 0) {
    fprintf(stderr,"Echec de l'enregistrement\n");
    exit(1);
  }
  svc_run(); /* le serveur est en attente de clients eventuels */
  return(0); /* on y passe jamais ! */
}
/*
Affichage :
  serveur: variable n (debut) : 1094861636,
  serveur: variable n (fin) : 1094861637,
  serveur: variable res : 1094861637
*/


