#include <GL/glut.h>
#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>

/* dimensions de la fenetre */
int width = 600;
int height = 400;

/*************************************************************************/
/* Fonctions callback */
/*************************************************************************/

void display()
{
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 60, (float)width/height, 1, 100);
  gluLookAt(2,2,4,0,0,0,0,1,0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	// dessins ? 

  glutSwapBuffers();
}

//------------------------------------------------------------

void keyboard(unsigned char keycode, int x, int y)
{
  printf("Touche frappee : %c (code ascii %d)\n",keycode,keycode);

  /* touche ECHAP */
  /*
	if (keycode==27)
	  exit(0);
	*/
  if (keycode=='q')
	  exit(0);

  glutPostRedisplay();
}

//------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
		printf("Bonjour !\n");
		//printf("Clic at %d %d\n",x,y);
		glutPostRedisplay();
    }
}


//------------------------------------------------------------

void idle()
{
  // animation du personnage ici

  glutPostRedisplay();
}

/*************************************************************************/
/* Fonction principale */
/*************************************************************************/

int main(int argc, char *argv[])
{
  /* Initialisations globales */
  glutInit(&argc, argv);

  /* Définition des attributs de la fenetre OpenGL */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  /* Placement de la fenetre */
  glutInitWindowSize(width, height);
  glutInitWindowPosition(50, 50);

  /* Création de la fenetre */
  glutCreateWindow("Transformations matricielles");

  /* Choix de la fonction d'affichage */
  glutDisplayFunc(display);

  /* Choix des fonctions de gestion du clavier */
  glutKeyboardFunc(keyboard);
  //glutSpecialFunc(special);

  /* Choix de la fonction de gestion de la souris */
  glutMouseFunc(mouse);

  /* Choix de la fonction qui sera appelée
     lorsqu'il n'y a pas d'autres événements */
  glutIdleFunc(idle);

  /* Boucle principale */
  glutMainLoop();

  /* Même si glutMainLoop ne rends JAMAIS la main,
     il faut définir le return, sinon
     le compilateur risque de crier */
  return 0;
}
