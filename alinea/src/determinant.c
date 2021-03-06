#include"determinant.h"

Matrix Extraction(Matrix m,int row,int column){
	
	Matrix extraite=newMatrix(m->nb_rows-1,m->nb_columns-1);
	int i,j;
	int x=0,y=0;

	for(i=0;i<m->nb_rows;i++){
		
		if(i==row){
			
			if(i==m->nb_rows-1){
				break;	
			}

			i++;
		}

		y=0;
		for(j=0;j<m->nb_columns;j++){

			if(j==column && j<m->nb_columns-1){
				j++;
				extraite->mat[x][y]=m->mat[i][j];
			}
			else 
				if(j!=column)
					extraite->mat[x][y]=m->mat[i][j];
			else
				j++;

			y++;
		}
		x++;
	}
	return extraite;
}


int Determinant(Matrix m){
	
	int i;
	float det=0;
	float tmp;
	if(m->nb_rows==2){
		tmp=(m->mat[0][0]*m->mat[1][1])-(m->mat[0][1]*m->mat[1][0]);
		deleteMatrix(m);
		return tmp;
	}
	else{
		for(i=0;i<m->nb_columns;i++)
			if(i%2==0)
				det+=m->mat[0][i]*Determinant(Extraction(m,0,i));
			else
				det+=-m->mat[0][i]*Determinant(Extraction(m,0,i));
	}
	deleteMatrix(m);
	return det;
}

