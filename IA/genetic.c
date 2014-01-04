#include "genetic.h"


void evaluate_population(population_t* pop,bmpgrey_t* image){
  int nb_ind=pop->nb_ind;
  int i;

  for (i = 0; i < nb_ind; i++) {
    pop->ind[i].entropy=entropy(image,pop->ind[i].L,pop->ind[i].D);
  }

}

//Enjambement
individu_t* crossing_over(individu_t* A,individu_t* B){
  static individu_t* C=NULL;

  if(C==NULL)
    C=create_individu();

  C->L=A->L;
  C->D=B->D;
  C->entropy=0.00;

  return C;
}
//choisi aleatoirement les individu de old (les bon de la generation precedante) et les
//cross entre eux et les place dans new, old est trie par la selection 
void crossing_from_population(population_t* old,population_t* new){
  int index_random1;
  int index_random2;
  int i;

  flush_population(new);
  for (i = 0; i < POPULATION_SIZE; i++) {
    population_add(crossing_over(&old->ind[i],&old->ind[(POPULATION_SIZE-1)-i]),new);
  }

  while(new->nb_ind<new->size){
    index_random1=rand()%POPULATION_SIZE;
    index_random2=rand()%POPULATION_SIZE;

    population_add(crossing_over(&old->ind[index_random1],&old->ind[index_random2]),new);

  }
}


void selection(population_t* old,population_t* new){
  static population_t* selected=NULL;
  int i;

  if(selected==NULL)
    selected=create_population(POPULATION_SIZE*2);

  flush_population(selected);
  quicksort_population(old,new,selected);

  //old contient maintenant tout les bon individu
  for(i=0;i<POPULATION_SIZE;i++)
    old->ind[i]=selected->ind[(POPULATION_SIZE*2-1)-i];

  //free tout les individu "mauvais", la moitie restante de selected

}

//mutation aleatoire de l'ordre de 1%
void mutate(individu_t* ind){
  int mut_chance;
  int threshold1=4;
  int threshold2=2;
  int signe;

  //Si un individu est un elite ,note de 200, alors pas de mutation
  //alors que si un individu est tres mauvais, il mutera beaucoup
  //threshold+=ind->note/200;
  mut_chance=rand()%100;

  if (mut_chance<threshold2) {
    signe=rand()%2;
    if (signe==0 && ind->L>0) {
      ind->L=ind->L-1;
    }
    else if(ind->L<15){
      ind->L=ind->L+1;
    }
  }
  else if(mut_chance<threshold1){
    signe=rand()%2;
    if (signe==0 && ind->D>5) {
      ind->D=ind->D-5;
    }
    else if(ind->D<250)
      ind->D=ind->D+5;
  }

}

void mutate_population(population_t* p){
  int i;
  int nb_ind=p->nb_ind;

  for (i = 0; i < nb_ind; i++) {
    mutate(&p->ind[i]);
  }
}

void genetic(bmpgrey_t* image,population_t* old,population_t* new ){

  //evalue les population
  evaluate_population(old,image);
  evaluate_population(new,image);

  //met tous les bon dans old 
  selection(old,new);
  //le resultat des crossing des adn de old vont dans new
  crossing_from_population(old,new);

  mutate_population(new);  

}

void flush_population(population_t* pop){
  pop->nb_ind=0;
}

bool population_add(individu_t* ind,population_t* pop){
  int nb_ind=pop->nb_ind;
  int size=pop->size;

  if(nb_ind<size){
    pop->ind[nb_ind]=*ind;
    pop->nb_ind++;
    return TRUE;
  }
  return FALSE;
}

void init(population_t* pop){
  pop->nb_ind=POPULATION_SIZE;
  int i;
  individu_t* ind=pop->ind;

  for (i = 0; i < POPULATION_SIZE; i++) {
    ind[i].L=(rand()%16)+1;
    ind[i].D=rand()%(256-ind[i].L);
  }
}

population_t* create_population(int size){

  population_t* new=malloc(sizeof(struct _population_t));
  new->ind=malloc(POPULATION_SIZE*sizeof(struct individu));

  new->size=POPULATION_SIZE;
  return new;
}

individu_t* create_individu(){
  individu_t* new=malloc(sizeof(struct individu));
  return new;
}
