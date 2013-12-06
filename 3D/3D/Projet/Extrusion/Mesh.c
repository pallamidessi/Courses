#include "Mesh.h"


Quad Q_new(Vector v1, Vector v2, Vector v3, Vector v4){
	Quad q;

	q._vertices[0]=(Vector) v1;
	q._vertices[1]=(Vector) v2;
	q._vertices[2]=(Vector) v3;
	q._vertices[3]=(Vector) v4;
	
	return q;
}

Vector Q_normal(Quad* q){
	Vector* vertices=q->_vertices;

	Vector V1=V_substract(vertices[1],vertices[0]);
	Vector V2=V_substract(vertices[2],vertices[1]);

	Vector normal=V_cross(V1,V2);
	Vector normal_unit=V_multiply((double)(1./V_length(normal)),normal);
	return normal_unit;
	
}

void Q_draw(Quad q,int mode)
{
	Vector p1=q._vertices[0];
	Vector p2=q._vertices[1];
	Vector p3=q._vertices[2];
	Vector p4=q._vertices[3];
	Vector normal=Q_normal(&q);
	
	if(mode==1){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
  glBegin(GL_POLYGON);
	
	glNormal3f(normal.x,normal.y,normal.z);
	glColor3d(0.5,0.3,0.1);
	
	glVertex3f(p1.x,p1.y,p1.z+325);
	glVertex3f(p2.x,p2.y,p2.z+325);
	glVertex3f(p3.x,p3.y,p3.z+325);
	glVertex3f(p4.x,p4.y,p4.z+325);
	glEnd();
}

void Q_print(Quad q){
	int i;
	for (i = 0; i < 4; i++) {
		printf("p%d : x=%f,y=%f,z=%f \n",i,q._vertices[i].x,q._vertices[i].y,q._vertices[i].z);
	}
}

//--------------------------------------------


void M_init(Mesh *M){
	M->_nb_quads=0;
	M->_is_filled=FALSE;
}
// initialise un Mesh (0 quads)

void M_addQuad(Mesh *M, Quad q){
	int pos=M->_nb_quads;
	bool is_filled=M->_is_filled;


	if (!is_filled) {
		M->_quads[pos]=(Quad) q;
		M->_nb_quads++;
	}

}
// ajoute au Mesh le quadrilatère q


void M_draw(Mesh *M,int mode){
	int i;

	for (i = 0; i < M->_nb_quads; i++) {
		Q_draw(M->_quads[i],mode);
	}

}
// dessine le Mesh M

void M_print(Mesh *M, char *message){
	int i;
	printf("Nombre de quadrilatere : %d \n",M->_nb_quads);

	if (M->_is_filled) {
		printf("Le maillage est clos\n");
	}
	else{
		printf("Le maillage n'est pas clos\n");
	}

	if (M->_is_smooth) {
		printf("Le maillage est lisse\n");
	}
	else{
		printf("Le maillage n'est pas lisse\n");
	}


	for (i = 0; i < M->_nb_quads; i++) {
		Q_print(M->_quads[i]);
	}
}
// Affiche sur une console les données
// relatives à M à des fins des debuggage.

void M_addSlice(Mesh *M, Polygon *P1, Polygon *P2){
	int i;
	int nb_vertices=P1->_nb_vertices;
	Vector* verticesP1=P1->_vertices;
	Vector* verticesP2=P2->_vertices;
	Quad current;
	Vector* verticesCurrent=current._vertices;

	for (i = 0; i < nb_vertices-1; i++) {
		verticesCurrent[0]=verticesP1[i];		
		verticesCurrent[1]=verticesP2[i];		
		verticesCurrent[2]=verticesP2[i+1];		
		verticesCurrent[3]=verticesP1[i+1];
		M_addQuad(M,current);
	}
		verticesCurrent[0]=verticesP1[i];		
		verticesCurrent[1]=verticesP2[i];		
		verticesCurrent[2]=verticesP2[0];		
		verticesCurrent[3]=verticesP1[0];
		M_addQuad(M,current);
}
// P1 et P2 sont supposés être des polygones ayant le même
// nombre N de sommets. Cette fonction ajoute à M les N quads
// obtenus en reliant les sommets de P1 à deux de P2.

void M_revolution(Mesh *M, Polygon *P, int nb_tranches){

	Polygon rot;
	Polygon tmp;
	int nb_vertices=P->_nb_vertices;
	int i,j;
	Vector* vertices;
	double angle=(double)((360.)/(double)(nb_tranches));
	//double newX,newZ;

	P_copy(P,&rot);
	P_copy(P,&tmp);
	vertices=rot._vertices;
	
	for (i = 1; i < nb_tranches; i++) {
		for (j = 0; j < nb_vertices; j++) {
			vertices[j]=V_rotateUy(vertices[j],angle*i);
		}
		M_addSlice(M,&tmp,&rot);
		P_copy(&rot,&tmp);
		P_copy(P,&rot);
	}

	M_addSlice(M,&tmp,P);
	//finir le mesh ?

}
// A partir d'un polygone P, cette fonction divise 2*pi en
// nb_tranches angles et ajoute à M les quads nécessaires pour
// réaliser une révolution de P autour de l'axe Y (cf figure 1).

void M_perlinExtrude(Mesh *QM, Polygon *p, int nb_slices){
	int i;
	Polygon tmp,tmp2;
	Vector perlin_from_polygon;
	
	P_copy(p,&tmp);
	P_copy(p,&tmp2);

	for (i = 0; i < 1; i++) {
		perlin_from_polygon=PRLN_vectorNoise(P_normal(&tmp));
		
		V_print(P_normal(&tmp),"normal tmp");
		V_print(perlin_from_polygon,"perlin");
		
		P_rotate(P_normal(&tmp),perlin_from_polygon,P_center(&tmp),&tmp);
		P_translate(&tmp,V_multiply(100,perlin_from_polygon));
		
		V_print(P_normal(&tmp),"normal tmp");
		
		M_addSlice(QM,&tmp,&tmp2);
		P_copy(&tmp,&tmp2);
	}
/*
		P_rotate(P_normal(&tmp),V_new(1,0,0),P_center(&tmp),&tmp2);
		M_addSlice(QM,&tmp,&tmp2);
		//P_rotate(P_normal(&tmp),V_new(0,1,0),P_center(&tmp),&tmp2);
		//M_addSlice(QM,&tmp,&tmp2);
		//
	*/
}
