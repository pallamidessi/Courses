#include "include.h"

int main (int argc, char **argv) {
  char *host = argv[1];
  enum clnt_stat stat;
  entiers2 res;
  entiers2 donnees = {13 , 5};
  stat = callrpc(/* host */ host,
		 /* prognum */ PROGNUM,
		 /* versnum */ VERSNUM,
		 /* procnum */ PROCNUM,
		 /* encodage argument */ (xdrproc_t) xdr_entiers2,
		 /* argument */ (char *)&donnees,
		 /* decodage retour */ (xdrproc_t)xdr_entiers2,
		 /* retour de la fonction distante */(char *)&res);

  if (stat != RPC_SUCCESS) { 
    fprintf(stderr, "Echec de l'appel distant\n");
    clnt_perrno(stat);      fprintf(stderr, "\n");
  } else {
    printf("client res : %d/%d (q:%d r:%d)\n",
	   donnees.x,donnees.y,res.x,res.y);
  }
  return(0);
}
