#include"tp1.h"
#include"determinant.h"
#include"gauss.h"
#include"inversion.h"
#include<time.h>

int main(){
	//int det;
	srand(time(NULL));

	Matrix test=newMatrix(3,3);
	Matrix test2;	

	remplissage(test);
	affichage(test);
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
	
	det=Determinant(test);
	printf("%d\n",det);
	printf("\n");

*/
//test=Pivot_Gauss(test);
//	affichage(test);
//	det=m_determinant(test);
//	printf("%d\n",det);

	Matrix copy=copie(test);
	affichage(copy);
	printf("\n");

	test2=inversion_comatrice(test);
	affichage(test2);
	printf("\n");

	test2=multiplication(copy,test2);	
	affichage(test2);
	printf("\n");
	
	deleteMatrix(test2);
	
	Matrix copy2=copie(copy);

	test2=inversion_gauss(copy);
	affichage(test2);
	printf("\n");


	test2=multiplication(copy2,test2);	
	affichage(test2);
	printf("\n");
	deleteMatrix(test2);
//	deleteMatrix(add);
//	deleteMatrix(mult);

	return 0;	
}
 
