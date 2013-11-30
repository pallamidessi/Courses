#include "utils.h"



//------------------------------------------------------------

void drawLine(Vector p1, Vector p2)
{
	glBegin(GL_LINES);
	glVertex3d(p1.x,p1.y,p1.z);
	glVertex3d(p2.x,p2.y,p2.z);
	glEnd();
}

//------------------------------------------------------------
void drawRepere()
{
	glColor3d(1,0,0);
	glBegin(GL_LINES);
	glVertex3d(325,325,0);
	glVertex3d(325+50,325,0);
	glEnd();

	glColor3d(0,1,0);
	glBegin(GL_LINES);
	glVertex3d(325,325,0);
	glVertex3d(325,325-50,0);
	glEnd();

	glColor3d(0,0,1);
	glBegin(GL_LINES);
	glVertex3d(325,325,0);
	glVertex3d(0,0,50+325);
	glEnd();
}


