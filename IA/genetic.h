#ifndef __GENETIC_H
#define __GENETIC_H

#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"
#include "entropy.h"

#define POPULATION_SIZE 100 

individu_t* crossing_over(individu_t* A,individu_t* B);
void evaluation(individu_t* ind,bmpgrey_t* image);
void selection(population_t* old,population_t* new);
void mutate(individu_t* ind);
void genetic(bmpgrey_t* image,population_t* old,population_t* new );
void flush_population(population_t* pop);
bool population_add(individu_t* ind,population_t* pop);
void init(population_t* pop);
individu_t* create_individu();
population_t* create_population(int size);

#endif
