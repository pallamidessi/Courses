#include"inversion.h"


Matrix inversion_comatrice(Matrix m){
	
	int i,j;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int determinant;
	Matrix co=newMatrix(rows,columns);
	Matrix tmp;
	E** comatrice=co->mat;

	/*Calcul de la comatrice en utilisant l'algorithme recursif */
	for(i=0;i<rows;i++){
		for(j=0;j<columns;j++){
			tmp=Extraction(m,i,j);
			if((i%2==0 && j%2==1) ||( i%2==1 && j%2==0))
				comatrice[i][j]-=Determinant(tmp);
			else
				comatrice[i][j]=Determinant(tmp);

		}
	}
	Matrix tmp2=copie(m);
	determinant=Determinant(m);
	m=tmp2;
	/* On multiplie la comatrice par l'inverse du determinant*/
	mult_scalar((float) 1/determinant,co);
	
	deleteMatrix(m);
	/* On prend la transposee de la comatrice*/
	co=transpose(co);	
	return co;
}

Matrix inversion_comatrice_op(Matrix m){
	
	int i,j;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int determinant;
	Matrix co=newMatrix(rows,columns);
	Matrix tmp;
	E** comatrice=co->mat;

	/*Calcul de la comatrice en utilisant la methode optimisee par le pivot de gauss */
	for(i=0;i<rows;i++){
		for(j=0;j<columns;j++){
			tmp=Extraction(m,i,j);
			if((i%2==0 && j%2==1) ||( i%2==1 && j%2==0))
				comatrice[i][j]-=m_determinant(tmp);
			else
				comatrice[i][j]=m_determinant(tmp);

			deleteMatrix(tmp);
		}
	}

	determinant=m_determinant(m);
	/* On multiplie la comatrice par l'inverse du determinant obtenu par le pivot de gauss*/
	mult_scalar((float) 1/determinant,co);
	
	deleteMatrix(m);

	/* On prend la transposee de la comatrice*/
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
		
		/*Choix du pivot et verification si la matrice est inversible*/
		row_max=k;

		for(l=k;l<rows;l++){
			if(m->mat[k][k]<m->mat[l][k])
				row_max=l;
		}
		
		permuter(m,row_max,k);
		permuter(inverse,row_max,k);

		pivot=m->mat[k][k];


		/*Descente du pivot de gauss*/
		for(i=k+1;i<rows;i++){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
				inverse->mat[i][j]-=(float) (inverse->mat[k][j]*coeff);
			}
		}
	}

	/*Remontee du pivot*/
	for(k=rows-1;k>=0;k--){
		
		pivot=m->mat[k][k];
		
		for(i=k-1;i>=0;i--){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k-1;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
				inverse->mat[i][j]-=(float) (inverse->mat[k][j]*coeff);
			}
		}
	}

	/*Par division,la matrice diagonal devient la matrice identite*/
	for(i=0;i<rows;i++){
		coeff=(float) 1/m->mat[i][i];
		multLigne(m,i,coeff);
		multLigne(inverse,i,coeff);
	}

	deleteMatrix(m);

	return inverse;	
}



	
	
	





