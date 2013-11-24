#include <stdio.h>
#include <stdlib.h>
#include "include.h"

Matrix new_matrix(int dim){
  int i;  
  Matrix new=(Matrix) malloc(sizeof(struct str_matrix));

  new->mat=(float**) malloc(sizeof(int*)*dim);
  for (i = 0; i < dim; i++) {
    new->mat[i]=malloc(sizeof(int)*dim);
  }
  new->dim=dim;
  return new;
}

Matrix_couple new_matrix_couple(int dim){
  Matrix_couple new=(Matrix_couple)malloc(sizeof(struct str_matrix_couple));
  new->a=new_matrix(dim);
  new->b=new_matrix(dim);
  new->dim=dim;
  return new;
}

void print_matrix(Matrix m){
  int i,j;
  int dim=m->dim;
  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      printf("%f ",m->mat[i][j]);
    }
    printf("\n");
  }

}

void print_matrix_couple(Matrix_couple m){

  print_matrix(m->a);
  printf("\n\n");
  print_matrix(m->b);
}

void fill_random_matrix(Matrix m){
  int i,j;
  int dim=m->dim;

  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      m->mat[i][j]=rand()%10;
    }
  }
}

void fill_random_matrix_couple(Matrix_couple m){
  fill_random_matrix(m->a);
  fill_random_matrix(m->b);
}

