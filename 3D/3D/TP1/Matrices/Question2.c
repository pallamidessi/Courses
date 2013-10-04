#include <GL/glut.h>
#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>

/*Coefficient d'homothetie*/
int coeff=1;
/*Angle de la rotation */
float theta1=0;
float theta2=0;
float theta3=0;

float zoom=4;
/* dimensions de la fenetre */
int width = 600;
int height = 400;

//------------------------------------------------------------

typedef struct
{
	float x, y, z;
} Vector;

//------------------------------------------------------------

Vector V_new(float x, float y, float z)
{
	Vector p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

/*************************************************************************/
/* Fonctions de dessin */
/*************************************************************************/

/* rouge vert bleu entre 0 et 1 */
void chooseRandomColor()
{
  glColor4d(drand48(),drand48(),drand48(),0);
}

//------------------------------------------------------------

void chooseColor(double r, double g, double b)
{
  glColor3d(r,g,b);
}

//------------------------------------------------------------

void drawLine(Vector p1, Vector p2)
{
	glBegin(GL_LINES);
	glVertex3d(p1.x,p1.y,p1.z);
	glVertex3d(p2.x,p2.y,p2.z);
	glEnd();
}

//------------------------------------------------------------

void drawQuad(Vector p1, Vector p2, Vector p3, Vector p4)
{
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_QUADS);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p2.y,p2.z);
  glVertex3f(p3.x,p3.y,p3.z);
  glVertex3f(p4.x,p4.y,p4.z);
  glEnd();
}

//------------------------------------------------------------

void drawCube()
{
	Vector cube[8];

	cube[0]=V_new(0,0,0);
	cube[1]=V_new(1,0,0);
	cube[2]=V_new(1,1,0);
	cube[3]=V_new(0,1,0);
	
	cube[4]=V_new(0,0,1);
	cube[5]=V_new(1,0,1);
	cube[6]=V_new(1,1,1);
	cube[7]=V_new(0,1,1);
	
	chooseRandomColor();
	drawQuad(cube[0],cube[1],cube[2],cube[3]);
	chooseRandomColor();
	drawQuad(cube[0],cube[3],cube[7],cube[4]);
	chooseRandomColor();
	drawQuad(cube[7],cube[6],cube[2],cube[3]);
	chooseRandomColor();
	drawQuad(cube[1],cube[2],cube[6],cube[5]);
	chooseRandomColor();
	drawQuad(cube[0],cube[1],cube[4],cube[5]);
	chooseRandomColor();
	drawQuad(cube[4],cube[5],cube[6],cube[7]);

	/*
	for (i = 0; i < 6; i++) {
		for (i = 0; i < 4; i++) {
			cube[i][j]=V_new()	
		}
	}
	*/
}


/*************************************************************************/
/* Fonctions callback */
/*************************************************************************/

void display()
{
	srand48(1); // Seulement pour la couleur aleatoire.

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60, (float)width/height, 1, 100);
	gluLookAt(2,2,zoom,0,0,0,0,1,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vector origin=V_new(0,0,0);
	Vector x=V_new(1,0,0);
	Vector y=V_new(0,1,0);
	Vector z=V_new(0,0,1);
	// Repere du monde
	// ...
	chooseColor(255,0,0);
	drawLine(origin,x);
	chooseColor(0,255,0);
	drawLine(origin,y);
	chooseColor(0,0,255);
	drawLine(origin,z);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/*Tete*/
	glPushMatrix();
	glTranslatef(0.2,1.8,0);
	glPushMatrix();
	glScalef(0.5,1,1);
	drawCube();
	glPopMatrix();
	glPopMatrix();

	/*Torse*/
	glPushMatrix();
	glTranslatef(0,0,0);
	glPushMatrix();
	glScalef(1,1.7,1);
	drawCube();

	/*Bras gauche*/
	
	glPushMatrix();
	glTranslatef(1.3,0,0);
	glPushMatrix();
	glRotatef(30,0,0,1);
	glPushMatrix();
	glScalef(0.3,1,1);
	drawCube();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	/*Bras droit*/
	/*
	glTranslatef(0,0,0);
	drawCube();
	glPopMatrix();
	
	glTranslatef(0,0,0);
	drawCube();
	glPopMatrix();
	
	glRotatef(theta2,1,0,0);
	glTranslatef(0,2,0);
	drawCube();
	glRotatef(theta3,0,0,1);
	glTranslatef(0,0,2);
	glScalef(2,1,1);
	drawCube();

	drawCube();
	*/
	glutSwapBuffers();

}

//------------------------------------------------------------

void keyboard(unsigned char keycode, int x, int y)
{
	printf("Touche frapee : %c (code ascii %d)\n",keycode, keycode);
	/* touche ECHAP */
	if (keycode==27)
		exit(0);
	else if (keycode=='a')
		coeff++;
	else if (keycode=='A')
		coeff--;
	else if (keycode=='z')
		theta1+=10;
	else if (keycode=='Z')
		theta1-=10;
	else if (keycode=='e')
		theta2+=10;
	else if (keycode=='E')
		theta2-=10;
	else if (keycode=='r')
		theta3+=10;
	else if (keycode=='R')
		theta3-=10;
	else if (keycode=='+')
		zoom++;
	else if (keycode=='-')
		zoom--;
	

	glutPostRedisplay();
}

//------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("Clic at %d %d\n",x,y);
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

	/* Choix de la fonction de redimensionnement de la fenetre */
//	glutReshapeFunc(reshape);

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
