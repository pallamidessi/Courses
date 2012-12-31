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

	/*Choix du pivot et verification si la matrice est inversible*/
	for(k=0;k<rows;k++){
		
		row_max=k;

		for(l=k;l<rows;l++){
			if(m->mat[k][k]<m->mat[l][k])
				row_max=l;
		}

		permuter(m,row_max,k);
		permuter(v,row_max,k);

		pivot=m->mat[k][k];
		/*Descente du pivot de gauss*/
		for(i=k+1;i<rows;i++){
			coeff=(float) ((float) m->mat[i][k]/(float) pivot);
			for(j=k;j<columns;j++){
				m->mat[i][j]-=(float) (m->mat[k][j]*coeff);
				v->mat[i][j]-=(float) (v->mat[k][j]*coeff);
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
				v->mat[i][0]-=(float) (v->mat[k][0]*coeff);
			}
		}
	}

	/*Par division,la matrice diagonal devient la matrice identite*/
	for(i=0;i<rows;i++){
		coeff=(float) 1/m->mat[i][i];
		multLigne(m,i,coeff);
		multLigne(v,i,coeff);
	}

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
Systeme saisie_systeme(){
	int nb_rows;
	int nb_columns;
	int i,j;
	
	printf("Nombre de colonne de la matrice : ");
	scanf("%d",&nb_columns);
	printf("Nombre de ligne de la matrice : ");
	scanf("%d",&nb_rows);

	Systeme s=newSystem(nb_rows,nb_columns);

	printf("On saisie le contenu de la matrice: \n");

	for(i=0;i<nb_rows;i++){
		for(j=0;j<nb_columns;j++){
			scanf("%f",&s->matrice->mat[i][j]);
		}
		printf("\n");
	}

	printf("On saisie les valeur du systeme: \n");
	for(i=0;i<nb_rows;i++){
		scanf("%f",&s->valeur->mat[i][0]);
	}
	return s;
}
void deleteSysteme(Systeme s){

	deleteMatrix(s->matrice);
	deleteMatrix(s->valeur);

	free(s);
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
	
