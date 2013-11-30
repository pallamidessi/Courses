
/*======================================================*\
	Wednesday September the 25th 2013
	Arash HABIBI
	Question1.c
	\*======================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include "Vector.h"
#include "Polygon.h"
#include "Mesh.h"
#include "bool.h"
#include "utils.h"

#define DIM2 0
#define DIM3 1
int dim=DIM2;

/* dimensions de la fenetre */
int width = 650;
int height = 650;

GLfloat p_light[4];

/*Polygon courant */
Polygon poly;
/*Mesh courant*/
Mesh* mesh;

/*Saisie*/
unsigned int stop=0;

//------------------------------------------------------------
void initShade()
{
	GLfloat mat_diffuse[] = {1,1,1,1.0};
	GLfloat mat_ambient[] = {0.1,0.1,0.1,0.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, p_light);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}


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

	if(dim==DIM2)	glOrtho(0,650,650,0,-1,1);
	else			gluPerspective( 40, (float)width/height, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Dessiner ici
	// ...

	// Repere du monde
	drawRepere();
	
	drawLine(V_new(0,325,0),V_new(650,325,0));
	drawLine(V_new(325,0,0),V_new(325,650,0));
	P_draw(&poly);

	glutSwapBuffers();
}

//------------------------------------------------------------

void keyboard(unsigned char keycode, int x, int y)
{
	// printf("Touche frapee : %c (code ascii %d)\n",keycode, keycode);

	if (keycode==27) // ECHAP
		exit(0);
	if (keycode=='c') {
		stop=1;
	}
	if (keycode=='c') {
		P_close(&poly);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------

void special(int keycode, int x, int y)
{
	int mod = glutGetModifiers();
	switch(keycode)
	{
		case GLUT_KEY_UP        : printf("Flèche haut\n"); break;
		case GLUT_KEY_DOWN      : printf("Flèche bas\n"); break;
		case GLUT_KEY_LEFT      : printf("Flèche gauche\n"); break;
		case GLUT_KEY_RIGHT     : printf("Flèche droite\n"); break;
		case GLUT_KEY_PAGE_UP   : printf("Flèche avant\n"); break;
		case GLUT_KEY_PAGE_DOWN : printf("Flèche arriere\n"); break;
		default : fprintf(stderr,"function special : unknown keycode %d\n",keycode); break;
	}
	if(mod==GLUT_ACTIVE_CTRL)
		glLightfv(GL_LIGHT0, GL_POSITION, p_light);
}

//------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	switch(button)
	{
		case GLUT_LEFT_BUTTON :
			if(state==GLUT_DOWN){
				fprintf(stderr,"Clic gauche\n");

				if(!poly._is_closed && stop==0)
					P_addVertex(&poly,V_new(x,y,0));

				if(!P_simple(&poly))
					P_removeLastVertex(&poly);

				if (P_isConvex(&poly)) {
					poly._is_convex=TRUE;
				}
				else{
					poly._is_convex=FALSE;
				}
			}
			break;

		case GLUT_MIDDLE_BUTTON :
			if(state==GLUT_DOWN){
				fprintf(stderr,"Clic milieu\n");
				stop=1;
			}
			break;

		case GLUT_RIGHT_BUTTON :
			if(state==GLUT_DOWN){
				fprintf(stderr,"Clic droit.\n");
				P_close(&poly);
			}
			break;
	}
	glutPostRedisplay();
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Transformations matricielles");
	glViewport(0, 0, width, height);
	glClearColor(0,0,0,0);
	P_init(&poly);
	
	if(V_segmentsIntersect(V_new(0,325,0),V_new(650,325,0),V_new(325,0,0),V_new(325,650,0)))
		printf("Intersection\n");
	else
		printf("pas intersection\n");
	glutDisplayFunc(display);
	//	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	p_light[0]=-10.0;
	p_light[1]=20.0;
	p_light[2]=0.0;
	p_light[3]=1.0;
	//Vector p_aim = V_new(0,0,-2.75);
	mesh = NULL;

	glutMainLoop();

	return 0;
}
