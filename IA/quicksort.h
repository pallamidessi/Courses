#ifndef __QUICKSORT_H
#define __QUICKSORT_H

#include "entropy.h"
#include "genetic.h"
class Quicksort{
  
  public:
  static void quicksort_population(Population* old,Population* new_,Population* selected);
  Quicksort();

  private:
  static void echanger(Population* tab,int i,int j);
  static void quicksort(Population* tab,int debut,int fin);
};
#endif
