
void P_init(Polygon *p){
	p->_nb_vertices=0;
	p->_is_closed=TRUE;
	p->_is_filled=FALSE;
	p->_is_convex=TRUE;
}
// initialise un polygone (0 sommets)

void P_copy(Polygon *original, Polygon *copie){
	copie->_nb_vertices=original->_nb_vertices;
	copie->_is_closed=original->_is_closed;
	copie->_is_filled=original->_is_filled;
	copie->_is_convex=original->_is_convex;

	for (i = 0; i < P_MAX_VERTICES; i++) {
		copie->_vertices[i]=(Vector)original->_vertices[i];
	}

}
// original et copie sont deux polygones déjà alloués.
// Cette fonction copie les donnée
// depuis original vers copie de façon à ce que les
// deux polygones soient identiques.

void P_addVertex(Polygon *P, Vector pos){
	int pos=P->_nb_vertices-1;
	bool is_filled=P->_is_filled;

	if (!is_filled) {
		P->_vertices[pos]=(Vector)pos;
	}

}
// ajoute un sommet au polygone P. Ce nouveau sommet est situé en pos.

void P_removeLastVertex(Polygon *P){
	P->_nb_vertices--;

	if (P->_nb_vertices==0) {
		P->_is_closed=TRUE;
	}
}
// enlève le dernier sommet de P

void P_draw(Polygon *P){
	Vector* tab=P->_vertices;
	int nb_vertices=P->_nb_vertices
		bool is_closed=P->_is_closed;
	Vector current;
	Vector current2;
	
	if (P->_is_convex) {
		//glColor rouge
		glColor3d(255,0,0);	
	}
	else{
		//glColor bleu
		glColor3d(0,0,255);	
	}

	if(is_closed){
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);

		for (i = 0; i < nb_vertices ; i++) {
			current=(Vector) tab[i];  
			glVertex3f(current.x,current.y,current.z);
		}

		glEnd();
	}
	else{
		if (nb_vertices>1) {
			for (i = 0; i < nb_vertices-1; i++) {
				current=(Vector) tab[i];  
				current=(Vector) tab[i+1];
				drawLine(current,current2);
			}
		}
	}
}
// dessine le polygone P

void P_print(Polygon *P, char *message){
	Vector* tab=P->_vertices;
	int nb_vertices=P->_nb_vertices
		Vector current;

	for (i = 0; i < nb_vertices; i++) {
		current=(Vector) tab[i];  
		printf("point %d : x %f, y %f, z %f \n",i,current.x,current.y,current.z);
	}
}
// Affiche sur une console les données de P
// à des fins de debuggage.

void  P_tournerAutourDeLAxeY(Polygon *P, double radians){}
// tourne tous les points de P d'un angle de radians
// radians autour de l'axe Y.


void P_close(Polygon *P){
	
	if (!P->_is_closed &&){
		if(P_simple(P))	
			P->_is_closed=TRUE;
	}
}

//Marche uniquement pour de s point avec z=0;
int P_isConvex(Polygon *P){
	int i;
	int nb_vertices=P->_nb_vertices;
	
	if (nb_vertices<3) {
		return TRUE;
	}
	
	Vector* vertices=P->_vertices;
	Vector V1;
	Vector V2;
	Vector crossProduct;

	V1=V_substract(vertices[i+1],vertices[i]);
	V2=V_substract(vertices[i+2],vertices[i+1]);
	crossProduct=V_cross(V1,v2);

	if(crossProduct.z>0)
		signe=1;
	else
		signe=0;

	for (i = 1; i < nb_vertices-2; i++) {
		V1=V_substract(vertices[i+1],vertices[i]);
		V2=V_substract(vertices[i+2],vertices[i+1]);
		crossProduct=V_cross(V1,v2);
		if(!((crossProduct.z>0 && signe==1) || (crossProduct.z<0 && signe==0)))
			return FALSE;
	}

	return TRUE;
}

int P_simple(Polygon *P){
	Vector* tab=P->_vertices;
	int nb_vertices=P->_nb_vertices;
	Vector last_vertex=vertices[_nb_vertices-1];
	Vector before_last_vertex=vertices[_nb_vertices-2];
	
	if (nb_vertices<3) {
		return TRUE;
	}

	for (i = 0; i < nb_vertices-2; i++) {
		if (V_segmentsIntersect(last_vertex,before_last_vertex,vertices[i],vertices[i+1])) {
			return FALSE;
		}
	}

	return TRUE;
}
#endif // __POLYGON_H__
