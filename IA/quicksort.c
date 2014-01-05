#include "quicksort.h"


//quicksort les element de old+new dans selected 
void Quicksort::quicksort_population(Population* old,Population* new_,Population* selected){
  int i;

  for (i = 0; i < 100; i++) {
    selected->population_add(&old->ind[i]);
  }

  for (i = 0; i < 100; i++) {
    selected->population_add(&new_->ind[i]);
  }

  Quicksort::quicksort(selected,0,(100*2)-1);

}

void Quicksort::echanger(Population* tab,int i,int j){
  Individu tmp=tab->ind[j];
  
  tab->ind[j]=tab->ind[i];
  tab->ind[i]=tmp;
}


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

