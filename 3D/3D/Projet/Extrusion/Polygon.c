
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
	int nb_vertices=P->_nb_vertices;
	bool is_closed=P->_is_closed;
	
	Vector current;
	Vector current2;

	if(is_closed){
		//glColor...
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


Vector P_center(Polygon *P){
	int i;
	int x=0,y=0,z=0;

	Vector* vertices=P->_vertices;
	int nb_vertices=P->_nb_vertices;

	for (i = 0; i <nb_vertices; i++) {
		x+=vertices[i].x;
		y+=vertices[i].y;
		z+=vertices[i].z;
	}

	return V_new(x/nb_vertices,y/nb_vertices,z/nb_vertices);
}

Vector P_normal(Polygon *P){
	if (!P->_is_closed) {
		return V_new(0,0,0);
	}
	
	Vector* vertices=P->_vertices;
	
	Vector V1=V_substract(vertices[1],vertices[0]);
	Vector V2=V_substract(vertices[2],vertices[1]);

	Vector normal=V_cross(V1,V2);
	Vector normal_unit=V_multiply(normal,1/V_length(normal));
	return normal_unit;
}


void P_translate(Polygon *P, Vector trans){
	int i;
	Vector* tab=P->_vertices;
	int nb_vertices=P->_nb_vertices;

	for (i = 0; i < nb_vertices; i++) {
		vertices[i].x=trans.x;
		vertices[i].y=trans.y;
		vertices[i].z=trans.z;
	}

}

#endif // __POLYGON_H__
