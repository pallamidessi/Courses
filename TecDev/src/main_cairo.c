/**
 * \file       logigraphe.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief      logigraphe.c .
 *
 * \details    Contient les fonction relative au logigraphe ou a leurs traitements.
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "grille_cairo.h"
#include "cairo_util.h"
#include "compteur.h"

#define SIZEX 900
#define SIZEY 900

int main(int argc, char *argv[])
{
	int decalX=0,decalY=0;
	grille test,l;

	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	KeySym keysym;	
	char buffer[100];

//initialisation de la fenetre x11
if(!(dpy=XOpenDisplay(NULL))) {
fprintf(stderr, "ERROR: Could not open display\n");
exit(1);
}

scr=DefaultScreen(dpy);
rootwin=RootWindow(dpy, scr);
win=XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0, BlackPixel(dpy, scr), BlackPixel(dpy, scr));

XStoreName(dpy, win, "Logigraphe");
XSelectInput(dpy, win, ExposureMask|KeyPressMask|ButtonPressMask|PointerMotionMask);
XMapWindow(dpy, win);

	// create cairo surface
cairo_surface_t *cs; 
cs=cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy,0), SIZEX, SIZEY);



l=menu(cs,e,dpy,keysym);
test=alloue_grille(l->N,l->M);


grille valL=compter_ligne(l);
grille valC=compter_colonne(l);

decalX=count_decalX(valL);
decalY=count_decalY(valC);

Affiche_jeu(cs,test,valL,valC,decalX,decalY,-1,-1,-1);
while(1) {
	XNextEvent(dpy, &e);
	if (e.type==Expose && e.xexpose.count<1){
		Affiche_jeu(cs,test,valL,valC,decalX,decalY,-1,-1,-1);
	}
	else 
		if (e.type==ButtonPress){	
			Affiche_jeu(cs,test,valL,valC,decalX,decalY,e.xbutton.x,e.xbutton.y,1);
			if(compare(test,l)==0){
				printf("bravo reussi !!!\n");
				break;
			}
		}
	else	
		if (e.type==MotionNotify){	
			Affiche_jeu(cs,test,valL,valC,decalX,decalY,e.xmotion.x,e.xmotion.y,0); 
		}
	else if (e.type==KeyPress && XLookupString(&e,buffer,100,&keysym,0)==1){
	  if (buffer[0]==27)
			break;
		}
}
												

cairo_surface_destroy(cs);
desalloue_grille(l);
desalloue_grille(test);
return 0;
}
