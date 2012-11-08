#include"tp1.h"
#include<time.h>

int main(){

	srand(time(NULL));

	Matrix test=newMatrix(3,3);
	Matrix test2=newMatrix(3,3);
	
	remplissage(test);
	remplissage(test2);
	
	affichage(test);
	printf("\n\n");
	affichage(test2);
	printf("\n\n");

	Matrix add=addition(test,test2);
	
	affichage(add);
	printf("\n\n");

	Matrix mult=multiplication(test,test2);
	affichage(mult);

	deleteMatrix(test);
	deleteMatrix(test2);
	deleteMatrix(add);
	deleteMatrix(mult);

	return 0;	
}

