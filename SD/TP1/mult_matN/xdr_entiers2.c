#include "include.h"

bool_t xdr_matrix(XDR *xdrs, Matrix e) {
  int i,j;
	int dim;

	if (xdrs->x_op==XDR_ENCODE){
		dim=e->dim;
		if(xdr_int(xdrs,&dim)!=TRUE)
			return FALSE;
	}else if (xdrs->x_op==XDR_DECODE){
		if(xdr_int(xdrs,&dim)!=TRUE)
			return FALSE;
		printf("%d\n",dim);
		e=new_matrix(dim);
		e->dim=dim;

	}

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if(xdr_float(xdrs,&(e->mat[i][j]))!=TRUE)
				return FALSE;
		}
	}
	return TRUE;
}

bool_t xdr_matrix_couple(XDR *xdrs, Matrix_couple e) {

	if(xdrs->x_op==XDR_ENCODE)
		return (xdr_matrix(xdrs,e->a)&&xdr_matrix(xdrs,e->b)&&xdr_int(xdrs,&e->dim));
	else if(xdrs->x_op==XDR_DECODE){
		printf("test\n");
		e=malloc(sizeof(struct str_matrix_couple));
		printf("test\n");
		xdr_matrix(xdrs,e->a);
		xdr_matrix(xdrs,e->b);
		xdr_int(xdrs,&e->dim);
	}
		return TRUE;
}
