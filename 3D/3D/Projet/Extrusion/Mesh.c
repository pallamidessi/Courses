#include "Mesh.h"


Quad Q_new(Vector v1, Vector v2, Vector v3, Vector v4){
	Quad q;

	q._vertices[0]=(Vector) v1;
	q._vertices[1]=(Vector) v2;
	q._vertices[2]=(Vector) v3;
	q._vertices[3]=(Vector) v4;
	
	return q;
}

void Q_draw(Quad q,int mode)
{
	Vector p1=q._vertices[0];
	Vector p2=q._vertices[1];
	Vector p3=q._vertices[2];
	Vector p4=q._vertices[3];
	
	glColor3d(0.5,0.3,0.1);
	if(mode==0){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);
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
	P_copy(P,&rot);
	int nb_vertices=rot._nb_vertices;
	int i,j;
	Vector* vertices=rot._vertices;
	double angle=(double)((720)/nb_tranches);

	double newX,newZ;
	P_copy(P,&tmp);
	for (i = 1; i < nb_tranches; i++) {

		for (j = 0; j < nb_vertices; j++) {
			newX=(vertices[j].x)*cos((double)(180/3.14)*angle*i)+vertices[j].z*sin((double)(180/3.14)*angle*i);
			newZ=vertices[j].z*cos((double)(180/3.14)*angle*i)-(vertices[j].x)*sin((double)(180/3.14)*angle*i);
			
			printf("X %f %f\n",vertices[j].x,newX);
			printf("Z %f %f\n",vertices[j].z,newZ);
			vertices[j].x=newX;
			vertices[j].z=newZ;
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

