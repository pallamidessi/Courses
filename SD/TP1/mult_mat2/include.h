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


typedef struct matrix{
	float **mat;
	int dim;
}str_matrix,Matrix*;

typedef struct{
	matrix a;
	matrix b;
} matrix_couple;

typedef struct { int x; int y; } entiers2;
bool_t xdr_matrix(XDR *, matrix *);
bool_t xdr_matrix_couple(XDR *, matrix_couple *);


#endif /* INCRPC */
