
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

/* dimensions de la fenetre */
int width = 600;
int height = 600;

//------------------------------------------------------------

void drawRepere()
{
	glColor3d(1,0,0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(1,0,0);
	glEnd();

	glColor3d(0,1,0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(0,1,0);
	glEnd();

	glColor3d(0,0,1);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(0,0,1);
	glEnd();
}

//------------------------------------------------------------

void display()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1,1,-1,1,-1,1);
	// gluPerspective( 60, (float)width/height, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// dessiner ici
	// ...

	drawRepere();

	glutSwapBuffers();
}

//------------------------------------------------------------

void keyboard(unsigned char keycode, int x, int y)
{
	printf("Touche frapee : %c (code ascii %d)\n",keycode, keycode);
	/* touche ECHAP */
	if (keycode==27)
		exit(0);

	glutPostRedisplay();
}

//------------------------------------------------------------

void special(int keycode, int x, int y)
{
	switch(keycode)
    {
    case GLUT_KEY_UP   : fprintf(stderr,"Fleche haut"); break;
    case GLUT_KEY_DOWN : fprintf(stderr,"Fleche bas"); break;
    case GLUT_KEY_LEFT : fprintf(stderr,"Fleche gauche"); break;
    case GLUT_KEY_RIGHT: fprintf(stderr,"Fleche droit"); break;
    case GLUT_KEY_PAGE_UP : fprintf(stderr,"Fleche qui s'éloigne"); break;
    case GLUT_KEY_PAGE_DOWN : fprintf(stderr,"Fleche qui s'approche"); break;
    default : fprintf(stderr,"function special : unknown keycode %d\n",keycode); break;
    }
}

//------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	switch(button)
    {
    case GLUT_LEFT_BUTTON :
		if(state==GLUT_DOWN)
			fprintf(stderr,"Clic gauche\n");
		break;

    case GLUT_MIDDLE_BUTTON :
		if(state==GLUT_DOWN)
			fprintf(stderr,"Clic milieu\n");
		break;

    case GLUT_RIGHT_BUTTON :
		if(state==GLUT_DOWN)
			fprintf(stderr,"Clic droit.\n");
		break;
    }
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
	glutDisplayFunc(display);
	//	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}
