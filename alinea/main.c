#include"tp1.h"
#include"determinant.h"
#include"gauss.h"
#include<time.h>

int main(){
	int det;
	srand(time(NULL));

	Matrix test=newMatrix(3,3);
	Matrix test2=newMatrix(3,3);
	
	remplissage(test);
	remplissage(test2);
	
	affichage(test);
	printf("\n\n");
	affichage(test2);
	printf("\n\n");

/*
	Matrix add=addition(test,test2);
	
	affichage(add);
	printf("\n\n");

	Matrix mult=multiplication(test,test2);
	affichage(mult);
		
	printf("\n\n");
	mult=Extraction(mult,0,0);
	affichage(mult);
	printf("\n\n");
	affichage(test);
	det=Determinant(test);
	printf("%d\n",det);
	printf("\n");

	test=Pivot_Gauss(test);
	affichage(test);
*/
	deleteMatrix(test);
	deleteMatrix(test2);
//	deleteMatrix(add);
//	deleteMatrix(mult);

	return 0;	
}

