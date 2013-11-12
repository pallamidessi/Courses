#include "include.h"

void xdr_freep(void* p){
  free(p);
}

bool_t xdr_matrix(XDR *xdrs, Matrix* e) {
  int i,j;
	int dim;
  Matrix m;

	if (xdrs->x_op==XDR_ENCODE){
    m=(*e);
		dim=m->dim;
		if(xdr_int(xdrs,&dim)!=TRUE)
			return FALSE;
	}else if (xdrs->x_op==XDR_DECODE){
		if(xdr_int(xdrs,&dim)!=TRUE)
			return FALSE;
		printf("%d\n",dim);
		m=new_matrix(dim);
		m->dim=dim;
    (*e)=m;
	}

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			
      if(xdr_float(xdrs,&(m->mat[i][j]))!=TRUE)
				return FALSE;
		}
	}
	return TRUE;
}

bool_t xdr_matrix_couple(XDR *xdrs, Matrix_couple* e) {

	if(xdrs->x_op==XDR_ENCODE){
    Matrix_couple m=(Matrix_couple)(*e);
		return (xdr_matrix(xdrs,&(m->a))&&xdr_matrix(xdrs,&(m->b))&&xdr_int(xdrs,&(m->dim)));
	}
  else if(xdrs->x_op==XDR_DECODE){
		printf("test\n");
		*e=malloc(sizeof(struct str_matrix_couple));
    printf("test\n");
    if(!xdr_matrix(xdrs,&(*e)->a))
      printf("rate 1!!\n");
    if(!xdr_matrix(xdrs,&(*e)->b))
      printf("rate 2!!\n");
		xdr_int(xdrs,&(*e)->dim);
	}
		return TRUE;
}
