#include <stdio.h>
#include <stdlib.h>
#include "include.h"

void print_matrix(Matrix m){
	int i,j;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			printf("%f ",m->mat[i][j]);
		}
		printf("\n");
	}

}

void print_matrix_couple(Matrix_couple m){
	
	print_matrix(&m->a);
	printf("\n\n");
	print_matrix(&m->b);
}

void random_fill_matrix(Matrix m){
	int i,j;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			m->mat[i][j]=rand()%10;
		}
	}
}

void rand_fill_matrix_couple(Matrix_couple m){
	random_fill_matrix(&m->a);
	random_fill_matrix(&m->b);
}

