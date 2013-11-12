#include "include.h"

int main (int argc, char **argv) {
  char *host = argv[1];
  enum clnt_stat stat;
  matrix res;
	int i,j;
	char choix;

  matrix_couple donnee;
  if (argc!=2) {
    printf("Usage : %s [IP server]\n",argv[0]);
    exit(1);
  }

  //Rempli aleatoirement une matrice 2x2
	srand(time(NULL));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			donnee.a.mat[i][j]=rand()%10;
			donnee.b.mat[i][j]=rand()%10;
		}
	}
	
	print_matrix_couple(&donnee);
	printf("Multiplication ou addition ? (m/a) ");
	scanf("%c",&choix);
	
	if(choix=='m'){
		stat = callrpc(/* host */ host,
			 /* prognum */ PROGNUM,
			 /* versnum */ VERSNUM,
			 /* procnum */ PROCNUM_MULT,
			 /* encodage argument */ (xdrproc_t) xdr_matrix_couple,
			 /* argument */ (char *)&donnee,
			 /* decodage retour */ (xdrproc_t)xdr_matrix,
			 /* retour de la fonction distante */(char *)&res);
	}
	else if(choix=='a'){
		stat = callrpc(/* host */ host,
			 /* prognum */ PROGNUM,
			 /* versnum */ VERSNUM,
			 /* procnum */ PROCNUM_ADD,
			 /* encodage argument */ (xdrproc_t) xdr_matrix_couple,
			 /* argument */ (char *)&donnee,
			 /* decodage retour */ (xdrproc_t)xdr_matrix,
			 /* retour de la fonction distante */(char *)&res);
	}
  if (stat != RPC_SUCCESS) { 
    fprintf(stderr, "Echec de l'appel distant\n");
    clnt_perrno(stat);      fprintf(stderr, "\n");
  } else {
		
    //lit le matrice resultat recu
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 2; j++) {
				printf("%f ",res.mat[i][j]);
			}
    	printf("\n");
		}

  }
  return(0);
}
