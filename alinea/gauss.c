#include"gauss.h"

Matrix permuter(Matrix m, int row1,int row2){
	E *tmp_ligne=m->mat[row1];

	m->mat[row1]=m->mat[row2];
	m->mat[row2]=tmp_ligne;
	
	return m;
}

Matrix multLigne(Matrix m,int row,float scalar){
	int j;

	for(j=0;j<m->nb_columns;j++){
		m->mat[row][j]*=scalar;	
	}
	
	return m;	
}

Matrix addMultLigne(Matrix m,int row1,int row2,float scalar){
	
	int j;

	for(j=0;j<m->nb_columns;j++){
		m->mat[row1][j]+=m->mat[row2][j]*scalar;	
	}
	
	return m;
}

Matrix Pivot_Gauss(Matrix m){
	
	int i,j,k,l;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int row_max;
	float pivot;
	float coeff;
	for(k=0;k<rows;k++){
		
		row_max=k;

		for(l=k;l<rows;l++){
			if(m->mat[k][k]<m->mat[l][k])
				row_max=l;
		}

		permuter(m,row_max,k);
		pivot=m->mat[k][k];
	//	printf("pivot %f \n",pivot);
		for(i=k+1;i<rows;i++){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
			}
		}
	//	printf("\n");
	//	affichage(m);
	//		printf("\n");
	}
	for(i=0;i<rows;i++){
		for(j=0;j<i;j++){
			m->mat[i][j]=0;
		}
	}

	return m;	
}


int m_determinant(Matrix m){
	
	int i;
	int det=1;
	
	for(i=0;i<m->nb_rows;i++){
		det*=m->mat[i][i];
	}	

	return det;
}
