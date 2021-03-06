#include "Mesh.h"


Quad Q_new(Vector v1, Vector v2, Vector v3, Vector v4){
  Quad q;

  q._vertices[0]=(Vector) v1;
  q._vertices[1]=(Vector) v2;
  q._vertices[2]=(Vector) v3;
  q._vertices[3]=(Vector) v4;

}

//--------------------------------------------


void M_init(Mesh *M){
  M->_nb_quads=0;
  M->is_filled=FALSE;
}
// initialise un Mesh (0 quads)

void M_addQuad(Mesh *M, Quad q){
  int pos=M->_nb_quads-1;
  bool is_filled=M->_is_filled;

  if (!is_filled) {
    M->_quad[pos]=(Quad) q;
  }

}
// ajoute au Mesh le quadrilatère q

void drawQuad(Quad q);

void M_draw(Mesh *M);
// dessine le Mesh M

void M_print(Mesh *M, char *message);
// Affiche sur une console les données
// relatives à M à des fins des debuggage.

void M_addSlice(Mesh *M, Polygon *P1, Polygon *P2);
// P1 et P2 sont supposés être des polygones ayant le même
// nombre N de sommets. Cette fonction ajoute à M les N quads
// obtenus en reliant les sommets de P1 à deux de P2.

void M_revolution(Mesh *M, Polygon *P, int nb_tranches);
// A partir d'un polygone P, cette fonction divise 2*pi en
// nb_tranches angles et ajoute à M les quads nécessaires pour
// réaliser une révolution de P autour de l'axe Y (cf figure 1).

#endif // __MESH_H__
void drawQuad(Quad q,int mode)
{
  Vector p1=q._vertices[0];
  Vector p2=q._vertices[1];
  Vector p3=q._vertices[2];
  Vector p4=q._vertices[3];
  
  if(mode==0){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glBegin(GL_QUADS);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p2.y,p2.z);
  glVertex3f(p3.x,p3.y,p3.z);
  glVertex3f(p4.x,p4.y,p4.z);
  glEnd();
}

