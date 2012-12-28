#include"inversion.h"


Matrix inversion_comatrice(Matrix m){
	
	int i,j;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int determinant;
	Matrix co=newMatrix(rows,columns);
	Matrix tmp;
	E** comatrice=co->mat;

	for(i=0;i<rows;i++){
		for(j=0;j<columns;j++){
			tmp=Extraction(m,i,j);
			if((i%2==0 && j%2==1) ||( i%2==1 && j%2==0))
				comatrice[i][j]-=Determinant(tmp);
			else
				comatrice[i][j]=Determinant(tmp);

			deleteMatrix(tmp);
		}
	}

	determinant=Determinant(m);
	
	mult_scalar((float) 1/determinant,co);
	
	deleteMatrix(m);
	co=transpose(co);	
	return co;
}



Matrix inversion_gauss(Matrix m){
	
	int i=0,j=0,k=0,l=0;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int row_max;
	float pivot;
	float coeff;

	Matrix inverse=identite(rows,columns);
	
	for(k=0;k<rows;k++){
		
		row_max=k;

		for(l=k;l<rows;l++){
			if(m->mat[k][k]<m->mat[l][k])
				row_max=l;
		}

		permuter(m,row_max,k);
		permuter(inverse,row_max,k);

		pivot=m->mat[k][k];
/*
		if(pivot==0){
			printf("Matrice non inversible");
			exit(0);
		}
*/
		for(i=k+1;i<rows;i++){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
				inverse->mat[i][j]-=(float) (inverse->mat[k][j]*coeff);
			}
		}
	affichage(inverse);
	printf("\n");
	}
	
//	affichage(m);
//	printf("\n");

	

	for(k=rows-1;k>=0;k--){
		
		pivot=m->mat[k][k];
		
		for(i=k-1;i>=0;i--){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k-1;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
				inverse->mat[i][j]-=(float) (inverse->mat[k][j]*coeff);
			}
		}
		
	affichage(inverse);
	printf("\n");
	}


	//affichage(m);
	//printf("\n");

	for(i=0;i<rows;i++){
		coeff=(float) 1/m->mat[i][i];
		multLigne(m,i,coeff);
		multLigne(inverse,i,coeff);
	}
	//affichage(inverse);
	printf("\n");

	deleteMatrix(m);

	return inverse;	
}



	
	
	





