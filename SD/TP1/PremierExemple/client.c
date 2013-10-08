#include "include.h"

int main (int argc, char **argv) {
  int res = 0x00000000;
  int n=0x41424344; 
  char *host = argv[1];
  enum clnt_stat stat;
  if (argc != 2) { printf("Usage: %s machine_serveur\n",argv[0]); exit(0); }
  printf("client: variable n (debut) : %d %s,\n",n,(char *)&n);
  stat = callrpc(/* host */ host,
		 /* prognum */ PROGNUM,
		 /* versnum */ VERSNUM,
		 /* procnum */ PROCNUM,
		 /* encodage argument */ (xdrproc_t) xdr_int,
		 /* argument */ (char *)&n,
		 /* decodage retour */ (xdrproc_t)xdr_int,
		 /* retour de la fonction distante */(char *)&res);

  if (stat != RPC_SUCCESS) { 
    fprintf(stderr, "Echec de l'appel distant\n");
    clnt_perrno(stat);      fprintf(stderr, "\n");
  } else {
    printf("client: variable n (fin) : %d,\n",n);
    printf("client: variable res : %d\n",res);
  }
  return(0);
}

/*  L'affichage sur un PC:
      client: variable n (debut) : 1094861636 DCBA,
      client: variable n (fin) : 1094861636,
      client: variable res : 1094861637

    L'affichage de la premiere ligne differe sur une SPARC :
      client: variable n (debut) : 1094861636 ABCD,


 Questions :

1) Ligne 7, pourquoi declare'e la variable 'res' static ?

2) Expliquez pourquoi l'affichage au niveau du client est
   different selon que l'on utilise une SPARC ou un PC.

3) Lignes 56 et 57, comprendre la declaration de type qui
   est faite.

4) Pourquoi la valeur finale de (*n) est differente
   sur le serveur et sur le client ?

5) Ecrire une application client/serveur permettant de 
   calculer le quotient et le reste de la division de 
   deux nombres en un seul appel distant.





*/
