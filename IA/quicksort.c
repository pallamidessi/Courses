#include "quicksort.h"


/*Quicksort the DNA of old and new_ in selected.
 * Selected must be initialized and alloc'd before*/
void Quicksort::quicksort_population(Population* old,Population* new_,Population* selected){
  int i;

  /*Adding DNA of old to selected*/
  for (i = 0; i < POPULATION_SIZE; i++) {
    selected->population_add(&old->ind[i]);
  }

  /*Adding DNA of new_ to selected*/
  for (i = 0; i < POPULATION_SIZE; i++) {
    selected->population_add(&new_->ind[i]);
  }

  Quicksort::quicksort(selected,0,(POPULATION_SIZE*2)-1);

}


/*Switch two DNA of a Population*/
void Quicksort::echanger(Population* tab,int i,int j){
  Individu tmp=tab->ind[j];
  
  tab->ind[j]=tab->ind[i];
  tab->ind[i]=tmp;
}


/*Quicksort using the entropy value*/
void Quicksort::quicksort(Population* tab,int debut,int fin){
  int i;

  if(debut<fin){
    float pivot=tab->ind[fin].entropy;
    int elementAechanger=debut;

    for(i=debut;i<fin;i++){
      if(tab->ind[i].entropy<pivot){
        echanger(tab,i,elementAechanger);
        elementAechanger++;
      }
    }
    Quicksort::echanger(tab,elementAechanger,fin);
    Quicksort::quicksort(tab,debut,elementAechanger-1);
    Quicksort::quicksort(tab,elementAechanger+1,fin);
  }
}

