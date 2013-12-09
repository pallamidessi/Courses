
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
#define NB_SLICE 100

/*Type d'affichage*/
int dim=DIM2;

/* dimensions de la fenetre */
int width = 650;
int height = 650;

/*Angle de la rotation */
float theta1=0;
float theta2=0;
float theta3=0;

/*Eclairage et camera*/
Vector p_aim;
float phi = -20;
float theta = 20;

GLfloat p_light[4];
float zoom=400;

/*Polygon courant */
Polygon poly;

/*Mesh courant*/
Mesh mesh;
int nb_slice=30;

/*Mode de dessin du mesh*/
int mode=1;

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

	if(dim==DIM2)	
		glOrtho(0,650,650,0,1,-2000);
	else{
		gluPerspective( 60, (float)width/height,1, 30000);
		gluLookAt(325,325,zoom,p_aim.x,p_aim.y,p_aim.z,0,-50,0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Rotation autour de l'axe X du repere
	glTranslatef(325,325,325);
	glRotatef(theta1,1,0,0);
	glTranslatef(-325,-325,-325);

	//Rotation autour de l'axe Y du repere
	glTranslatef(325,325,325);
	glRotatef(theta2,0,1,0);
	glTranslatef(-325,-325,-325);

	//Rotation autour de l'axe Z du repere
	glTranslatef(325,325,325);
	glRotatef(theta3,0,0,1);
	glTranslatef(-325,-325,-325);
	
	//Dessin du repere et des objets	
	drawRepere();
	P_draw(&poly);
	M_draw(&mesh,mode);

	glutSwapBuffers();
}

//------------------------------------------------------------

void keyboard(unsigned char keycode, int x, int y)
{
	// printf("Touche frapee : %c (code ascii %d)\n",keycode, keycode);

	if (keycode==27) // ECHAP
		exit(0);
	/*Ferme le polygone si possible*/
	else if (keycode=='c') {
		if(P_close(&poly))
			stop=1;
	}
	/*revolution du polygone */
	else if (keycode=='r') { 
		M_revolution(&mesh,&poly,nb_slice);
	}
	/*Change le mode de projection*/
	else if (keycode=='a') {
		if (dim==DIM3) {
			dim=DIM2;
		}
		else{
			dim=DIM3;
		}
	}
	/*Change le mode de dessin*/
	else if (keycode=='m') {
		if (mode==1) {
			mode=0;
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
		}
		else{
			mode=1;
			glEnable(GL_DEPTH_TEST);
			initShade();
		}
	}
	/*Zoom de la camera*/
	else if (keycode=='+')
		zoom+=4;
	else if (keycode=='-')
		zoom-=4;
	/*Ajoute/enleve des tranches a la revolution*/
	else if (keycode=='s'){
		nb_slice++;
		M_init(&mesh);
		M_revolution(&mesh,&poly,nb_slice);
	}
	else if (keycode=='S'){
		nb_slice--;
		M_init(&mesh);
		M_revolution(&mesh,&poly,nb_slice);
	}
	/*Extrusion selon des vecteur de perlin
	 * NON FONCTIONNELLE*/
	else if (keycode=='e'){
		if (poly._is_closed) {
			M_perlinExtrude(&mesh,&poly,nb_slice);
		}
	}
	/*Change les angle de rotation autour du repere*/
	/*pour ux*/
	else if (keycode=='x')
		theta1+=10;
	else if (keycode=='X')
		theta1-=10;
	/*pour uy*/
	else if (keycode=='y')
		theta2+=10;
	else if (keycode=='Y')
		theta2-=10;
	/*pour uz*/
	else if (keycode=='z')
		theta3+=10;
	else if (keycode=='Z')
		theta3-=10;

	glutPostRedisplay();
}

//------------------------------------------------------------

void special(int keycode, int x, int y)
{
	/*Controle de la camera et de l'eclairage
	 * CTRL + fleche : modification de la position de la camera
	 * MAJ  + fleche : modification de la position de l'eclairage
	*/

	int mod = glutGetModifiers();
	switch(keycode)
	{
		case GLUT_KEY_UP        : if(mod==GLUT_ACTIVE_CTRL) p_light[1]+=4; else if(mod==GLUT_ACTIVE_SHIFT) p_aim.y+=1; else theta-= 10; break;
		case GLUT_KEY_DOWN      : if(mod==GLUT_ACTIVE_CTRL) p_light[1]-=4; else if(mod==GLUT_ACTIVE_SHIFT) p_aim.y-=1; else theta+= 10; break;
		case GLUT_KEY_LEFT      : if(mod==GLUT_ACTIVE_CTRL) p_light[0]-=4; else if(mod==GLUT_ACTIVE_SHIFT) p_aim.x-=1; else phi-= 10; break;
		case GLUT_KEY_RIGHT     : if(mod==GLUT_ACTIVE_CTRL) p_light[0]+=4; else if(mod==GLUT_ACTIVE_SHIFT) p_aim.x+=1; else phi+= 10; break;
		case GLUT_KEY_PAGE_UP   : if(mod==GLUT_ACTIVE_CTRL) p_light[2]-=4; else p_aim.z-=4; break;
		case GLUT_KEY_PAGE_DOWN : if(mod==GLUT_ACTIVE_CTRL) p_light[2]+=4; else p_aim.z+=4; break;
		default : fprintf(stderr,"function special : unknown keycode %d\n",keycode); break;
	}
	if(mod==GLUT_ACTIVE_CTRL)
		glLightfv(GL_LIGHT0, GL_POSITION, p_light);
}

//------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	switch(button)
	{	/*Clic gauche
		 *Ajout d'un vertex au polygone
		*/
		case GLUT_LEFT_BUTTON :
			if(state==GLUT_DOWN){
				fprintf(stderr,"Clic gauche\n");
				
				/*Si le polygone n'est ferme et qu'on est toujours en mode "dessin" */
				if(!poly._is_closed && stop==0) 
					P_addVertex(&poly,V_new(x,y,0));
				
				/*Si le polygone obtenu apres ajout n'est pas simple,on annule l'ajout*/
				if(!P_simple(&poly))
					P_removeLastVertex(&poly);
				
				/*Test de la concavite/convexite apres le potentiel ajout*/
				if (P_isConvex(&poly)) {
					poly._is_convex=TRUE;
				}
				else{
					poly._is_convex=FALSE;
				}
			}
			break;
		/*Clic milieu
		 * active/desactive le mode "dessin" de polygone
		*/
		case GLUT_MIDDLE_BUTTON :
			if(state==GLUT_DOWN){
				if(stop)
					stop=0;
				else
					stop=1;	
			}
			break;
		/*Clic doit 
		 *	Essaye de fermer le polygone
		*/
		case GLUT_RIGHT_BUTTON :
			if(state==GLUT_DOWN){
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
	glutCreateWindow("Extrusion");
	glViewport(0, 0, width, height);
	glClearColor(0,0,0,0);
	P_init(&poly);
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	p_light[0]=-10.0;
	p_light[1]=20.0;
	p_light[2]=0.0;
	p_light[3]=1.0;
	p_aim = V_new(325,325,325);

	glutMainLoop();

	return 0;
}
