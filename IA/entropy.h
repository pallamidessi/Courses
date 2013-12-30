/**
 * @file entropy.c
 * @author Pallamidessi Joseph
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
**/  
#ifndef __ENTROPY_H
#define __ENTROPY_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmpfile.h"

struct _bmpfile2 {
  int  length;
  int  height;
  int color_depth;
  double* histogram;
  int* greyscale;
  int nb_color;
};

typedef struct individu{
  float L;
  int D;
}individu_t;

typedef struct _bmpfile2 bmpgrey_t;

bmpgrey_t* simple_import(char* filename);
void simple_export(bmpgrey_t* image, char* bmpname);
void create_histo_tab(bmpgrey_t* image);
double entropy(bmpgrey_t* image,double* histo,int width);
individu_t* color_reduction_4bit(bmpgrey_t* image);

double log2(double x);

#endif
