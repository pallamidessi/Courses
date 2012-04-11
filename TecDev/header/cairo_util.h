#ifndef _grille_util_h
#define _grille_util_h

#include <stdio.h>
#include <stdlib.h>
#include "grille_cairo.h"
typedef struct _rectangle{
int x;
int y;
int width;
int height;
} rectangle_t;

void Affiche_jeu(cairo_surface_t* surface,grille l,grille Ligne,grille Col,int decalX,int
decalY,int Sx,int Sy,int bouton);
void carreSelection(cairo_t* mask,int Sx,int Sy,grille l,int decalX,int decalY,int bouton);
void tracer_grille(cairo_t* mask,grille l,int decalX,int decalY);

#endif
