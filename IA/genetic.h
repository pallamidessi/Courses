#ifndef __GENETIC_H
#define __GENETIC_H

#include <cstdio>
#include <cstdlib>
#include "quicksort.h"
#include "entropy.h"

#define POPULATION_SIZE 100 

class Genetic{
  public:
  Population* old;
  Population* new_;
  Bmpgrey* image;

  Genetic(Population* old,Population* new_,Bmpgrey* image);

  Individu* crossing_over(Individu* A,Individu* B);
  
  void crossing_from_population();
  void selection();
  void genetic();
};
#endif
