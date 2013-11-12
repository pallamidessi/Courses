#include "include.h"

int main (int argc, char **argv) {
  char *host = argv[1];
  enum clnt_stat stat;
  Matrix res;
	int i,j,dim;
	char choix;

	srand(time(NULL));
   
  if (argc!=2) {
    printf("Usage :%s [IP serveur]\n",argv[0]);
  }

  Matrix_couple donnee=new_matrix_couple(rand()%10+1);
	fill_random_matrix_couple(donnee);

	print_matrix_couple(donnee);
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
		
    dim=res->dim;
		for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++) {
				printf("%f ",res->mat[i][j]);
			}
    	printf("\n");
		}

  }
  return(0);
}
