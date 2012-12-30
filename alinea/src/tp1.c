#include"tp1.h"


Matrix newMatrix(int nb_rows,int nb_columns){
	int i,v;

	Matrix m=NULL;
	m=malloc(sizeof(str_matrix));
	m->mat= malloc((nb_rows*sizeof(E*)));
			
	for (i=0;i<nb_rows;i++)
		m->mat[i]=malloc(nb_columns*sizeof(E));

	for (i=0;i<nb_rows;i++){
		 for (v=0;v<nb_columns;v++){
				 m->mat[i][v]=0;
		  }
	}

	m->nb_rows=nb_rows;
	m->nb_columns=nb_columns;

	return m;
}


E getElm(Matrix m,int row,int column){
	return m->mat[row][column];
}


void setElm(Matrix m,int row,int column,E val){
	m->mat[row][column]=val;
}

void deleteMatrix(Matrix m){
	int i;

	for(i=0;i<m->nb_rows;i++){
		free(m->mat[i]);
	}

	free(m);
}	

Matrix identite(int nb_rows,int nb_columns){
	
	int i;
	Matrix id=newMatrix(nb_rows,nb_columns);

	for(i=0;i<nb_rows;i++){
		id->mat[i][i]=1;
	}

	return id;
}

int isSymetric(Matrix m){
	int i,j;

	for(i=0;i<m->nb_rows;i++){
		for(j=0;j<m->nb_columns;j++){
			if(m->mat[i][j]!=m->mat[j][i])
				return 0;
		}	
	}
return 1;
}

int isSquare(Matrix m){
	if(m->nb_columns==m->nb_rows)
		return 1;
	else
		return 0;
}

Matrix copie(Matrix m){
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	Matrix copy=newMatrix(rows,columns);
	E** matrice=m->mat;
	E** m_copy=copy->mat;
	int i,j;

	for(i=0;i<rows;i++){
		for(j=0;j<columns;j++){
			m_copy[i][j]=matrice[i][j];	
		}
	}

	return copy;	
}

Matrix transpose(Matrix m){
	int i,j;
	Matrix transpose=newMatrix(m->nb_columns,m->nb_rows);

	for(i=0;i<m->nb_rows;i++){
		for(j=0;j<m->nb_columns;j++){
			transpose->mat[j][i]=m->mat[i][j];
		}
	}

	return transpose;
}

Matrix addition(Matrix a,Matrix b){
	int i,j;

	if(a->nb_columns!=b->nb_columns
		&&a->nb_rows!=b->nb_rows)
			return NULL;

	Matrix resultat=newMatrix(a->nb_columns,a->nb_rows);

	for(i=0;i<a->nb_rows;i++){
		for(j=0;j<a->nb_columns;j++){
			resultat->mat[i][j]=a->mat[j][i]+b->mat[i][j];
		}
	}

	return resultat;
}

Matrix multiplication(Matrix a,Matrix b){
	int i,j,k;

	if(a->nb_columns!=b->nb_rows)
		return NULL;

	Matrix resultat=newMatrix(a->nb_rows,b->nb_columns);

	for(i=0;i<resultat->nb_rows;i++){
		for(j=0;j<resultat->nb_columns;j++){
			for(k=0;k<a->nb_columns;k++)
				resultat->mat[i][j]+=a->mat[i][k]*b->mat[k][j];
		}
	}

	return resultat;
}

Matrix mult_scalar(E scalar,Matrix m){
	int i,j;	
	
	for(i=0;i<m->nb_rows;i++){
		for(j=0;j<m->nb_columns;j++){
			m->mat[i][j]*=scalar;
		}
	}	

return m; 	
}

void remplissage(Matrix m){
	int i,j;	

	for(i=0;i<m->nb_rows;i++){
		for(j=0;j<m->nb_columns;j++){
			m->mat[i][j]=rand()%10;
		}
	}		
}

Matrix saisie(){
	int nb_rows;
	int nb_columns;
	int i,j;
	
	printf("nombre de colonne de la matrice : ");
	scanf("%d",&nb_columns);
	printf("nombre de ligne de la matrice : ");
	scanf("%d",&nb_rows);

	printf("\n");
	Matrix new=newMatrix(nb_rows,nb_columns);
	
	for(i=0;i<nb_rows;i++){
		for(j=0;j<nb_columns;j++){
			scanf("%f ",&new->mat[i][j]);
		}
		printf("\n");
	}
	
	return new;
}

void affichage(Matrix m){
	int i,j;	

	for(i=0;i<m->nb_rows;i++){
		for(j=0;j<m->nb_columns;j++){
			printf(" %f",m->mat[i][j]);
		}
		printf("\n");
	}	
}



