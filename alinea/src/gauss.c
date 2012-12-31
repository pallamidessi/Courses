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

int Pivot_Gauss(Matrix m){
	
	int i,j,k,l;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int row_max;
	float pivot;
	float coeff;
	int signe=0;


	for(k=0;k<rows;k++){
		
		/*Choix du pivot et verification si la matrice est inversible*/
		row_max=k;

		for(l=k;l<rows;l++){
			if(m->mat[k][k]<m->mat[l][k])
				row_max=l;
		}

		permuter(m,row_max,k);
		/*Lors d'un permutation on garde la puissance du coefficient (-1) pour avoir le bon signe du determinant*/
		if(row_max!=k)
			signe+=row_max+k;

		pivot=m->mat[k][k];
		/*Descente du pivot de gauss*/
		for(i=k+1;i<rows;i++){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
			}
		}

	}
	for(i=0;i<rows;i++){
		for(j=0;j<i;j++){
			m->mat[i][j]=0;
		}
	}
	
	return signe;	

}


int m_determinant(Matrix m){
	
	int i;
	int signe=0;
	int det=1;

	if(m->nb_rows==2 && m->nb_columns==2)
		return (m->mat[0][0]*m->mat[1][1])-(m->mat[0][1]*m->mat[1][0]);
	
	signe=Pivot_Gauss(m);

	for(i=0;i<m->nb_rows;i++){
		det*=m->mat[i][i];
	}
	
	if(signe%2==1)
		det*=-1;

	return det;
}
