#include"determinant.h"

Matrix Extraction(Matrix m,int row,int column,int size){
	Matrix extraite=newMatrix(size,size);
	int i,j;
	int x,y;

	if(row>m->nb_rows||
		column>m->nb_columns||
		size>m->nb_columns){
			printf("erreur extraction : dimension\n");
			exit(1);
		}
	
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			x=i+row;
			y=j+column;

			if(x>m->nb_rows)
				x=x%m->nb_rows;
			else if(y>m->nb_columns)
				y=x%m->nb_columns;

			extraite->mat[i][j]=m->mat[x][y];
		}
	}
	
	return extraite;
}


int Determinant(Matrix m){
	
	int i;
	int det=0;
	
	if(m->nb_rows==2){
		return (m->mat[0][0]*m->mat[1][1])-(m->mat[0][1]*m->mat[1][0]);
	}
	else{
		for(i=0;i<m->nb_columns;i++)
			det+=m->mat[0][i]*Determinant(Extraction(m,1,i,m->nb_columns-1));
	}

	return det;
}

