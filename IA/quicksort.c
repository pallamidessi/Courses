#include "quicksort.h"


//quicksort les element de old+new dans selected 
void quicksort_population(population_t* old,population_t* new,population_t* selected){
  int i;

  for (i = 0; i < POPULATION_SIZE; i++) {
    population_add(&old->ind[i],selected);
  }
  for (i = 0; i < POPULATION_SIZE; i++) {
    population_add(&new->ind[i],selected);
  }

  quicksort(selected,0,POPULATION_SIZE*2-1);

}

void echanger(population_t* tab,int i,int j){
  individu_t tmp=tab->ind[j];
  
  tab->ind[j]=tab->ind[i];
  tab->ind[i]=tmp;
}


void quicksort(population_t* tab,int debut,int fin){
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
    echanger(tab,elementAechanger,fin);
    quicksort(tab,debut,elementAechanger-1);
    quicksort(tab,elementAechanger+1,fin);
  }
}

