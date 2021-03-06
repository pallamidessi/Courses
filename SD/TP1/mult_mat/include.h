#ifndef INCRPC
#define INCRPC  

#include <time.h>
#include <stdlib.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/rpc.h>
#define PROGNUM 0x20000100
#define VERSNUM 1
#define PROCNUM_MULT 1 //Multiplication
#define PROCNUM_ADD 2 //Addition

typedef struct str_matrix{
  float mat[2][2];
}matrix,*Matrix;

typedef struct str_couple_matrix{
  matrix a;
  matrix b;
} matrix_couple,*Matrix_couple;

typedef struct { int x; int y; } entiers2;
bool_t xdr_matrix(XDR *, Matrix);
bool_t xdr_matrix_couple(XDR *, Matrix_couple );

void print_matrix(Matrix m);
void print_matrix_couple(Matrix_couple m);
void fill_random_matrix(Matrix m);
void fill_random_matrix_couple(Matrix_couple m);

#endif /* INCRPC */
