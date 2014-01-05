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
#include <cstdio>
#include <cstdlib>
#include <cmath>
//#include "bmpfile.h"

class Individu;

class Bmpgrey {
  public:
  int  length;
  int  height;
  int color_depth;
  double* histogram;
  int* greyscale;
  int nb_color;
  
  Bmpgrey(char* filename);
  //void simple_export( char* bmpname);
  void create_histo_tab();
  double entropy(int width_window,int decal_window);
  static double log2(double x);
  Individu* color_reduction_4bit();
};

class Individu{
  public:
  int L;
  int D;
  double entropy;

  Individu();
  void mutate();
  void evaluate(Bmpgrey* image);
};

class Population {
  public:
  Individu* ind;
  int nb_ind;
  int size;

  void init();
  Population(int size);
  void flush_population();
  bool population_add(Individu* ind);
  void mutate_population();
  void evaluate_population(Bmpgrey* image);
};

#endif
