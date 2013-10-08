#include "include.h"

bool_t xdr_matrix(XDR *xdrs, Matrix e) {
  int i,j;

	for (i = 0; i < 2; i++) {
  	for (j = 0; j < 2; j++) {
  		if(xdr_float(xdrs,&(e->mat[i][j]))!=TRUE)
				return FALSE;
  	}
  }
	return TRUE;
}

bool_t xdr_matrix_couple(XDR *xdrs, Matrix_couple e) {
	return (xdr_matrix(xdrs,&e->a)&&xdr_matrix(xdrs,&e->b));
}
