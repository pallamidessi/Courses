#include"resol.h"


Systeme newSystem(int nb_rows,int nb_columns){
	Systeme s=malloc(sizeof(str_systeme));
	s->matrice=newMatrix(nb_rows,nb_columns);
	s->valeur=newMatrix(nb_rows,1);

	return s;
}

void remplissage_systeme(Systeme s){
	
	remplissage(s->matrice);
	remplissage(s->valeur);

}



Systeme resolution(Systeme s){
	Matrix m=s->matrice;
	Matrix v=s->valeur;

	int i=0,j=0,k=0,l=0;
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
		permuter(v,row_max,k);

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
				v->mat[i][j]-=(float) (v->mat[k][j]*coeff);
			}
		}
	affichage_systeme(s);
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
				v->mat[i][0]-=(float) (v->mat[k][0]*coeff);
			}
		}
		
	affichage_systeme(s);
	printf("\n");
	}


	//affichage(m);
	//printf("\n");

	for(i=0;i<rows;i++){
		coeff=(float) 1/m->mat[i][i];
		multLigne(m,i,coeff);
		multLigne(v,i,coeff);
	}
	//affichage(inverse);
	printf("\n");


	return s;	
}

void affichage_systeme(Systeme s){
	int i,j;	
	Matrix m=s->matrice;
	Matrix v=s->valeur;

	for(i=0;i<m->nb_rows;i++){
		for(j=0;j<m->nb_columns;j++){
			printf(" %f",m->mat[i][j]);
		}
		printf(" | %f",v->mat[i][0]);
		printf("\n");
	}

}


void valeur_propre(Matrix m){
	
	
	int i,j,k,l;
	int rows=m->nb_rows;
	int columns=m->nb_columns;
	int row_max;
	float pivot;
	float coeff;


	Matrix id=identite(rows,columns);

	for(k=0;k<rows;k++){
		
		row_max=k;

		for(l=k;l<rows;l++){
			if(m->mat[k][k]<m->mat[l][k])
				row_max=l;
		}

		permuter(m,row_max,k);
		permuter(id,row_max,k);
		pivot=m->mat[k][k];
	//	printf("pivot %f \n",pivot);
		for(i=k+1;i<rows;i++){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
				id->mat[i][j]-=(float) (id->mat[k][j]*coeff);
			}
		}
		affichage(m);
		printf("val \n");
		affichage(id);
		printf("lambda \n");
	}

	float propre[rows];

	for(i=0;i<rows;i++){
		if(id->mat[i][i]>=0){
			propre[i]=(float) ((float) -m->mat[i][i]/ (float) id->mat[i][i]);
			printf("valeur propre : %f\n",propre[i]);
		}
	}

}
	
