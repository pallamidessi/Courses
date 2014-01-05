#include "genetic.h"

Genetic::Genetic(Population* old,Population* new_,Bmpgrey* image){
  this->old=old;
  this->new_=new_;
  this->image=image;
}

void Population::evaluate_population(Bmpgrey* image){
  int i;

  for (i = 0; i < nb_ind; i++) {
    ind[i].evaluate(image);;
  }
}

void Individu::evaluate(Bmpgrey* image){
  entropy=image->entropy(L,D);
}

//Enjambement
Individu* Genetic:: crossing_over(Individu* A,Individu* B){
  static Individu* C=NULL;

  if(C==NULL)
    C=new Individu();

  C->L=A->L;
  C->D=B->D;
  C->entropy=0.00;

  return C;
}
//choisi aleatoirement les individu de old (les bon de la generation precedante) et les
//cross entre eux et les place dans new, old est trie par la selection 
void Genetic::crossing_from_population(){
  int index_random1;
  int index_random2;
  int i;

  new_->flush_population();
  for (i = 0; i < POPULATION_SIZE; i++) {
    new_->population_add(crossing_over(&old->ind[i],&old->ind[(POPULATION_SIZE-1)-i]));
  }

  while(new_->nb_ind<new_->size){
    index_random1=rand()%POPULATION_SIZE;
    index_random2=rand()%POPULATION_SIZE;

    new_->population_add(crossing_over(&old->ind[index_random1],&old->ind[index_random2]));

  }
}


void Genetic::selection(){
  static Population* selected=NULL;
  int i;

  if(selected==NULL)
    selected=new Population(100*2);

  selected->flush_population();
  Quicksort::quicksort_population(old,new_,selected);

  //old contient maintenant tout les bon individu
  for(i=0;i<100;i++)
    old->ind[i]=selected->ind[(100*2-1)-i];

}

//mutation aleatoire de l'ordre de 1%
void Individu::mutate(){
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
    if (signe==0 && L>0) {
      L--;
    }
    else if(L<15){
      L++;
    }
  }
  else if(mut_chance<threshold1){
    signe=rand()%2;
    if (signe==0 && D>5) {
      D-=5;
    }
    else if(D<250)
      D+=5;
  }

}

void Population::mutate_population(){
  int i;
 

  for (i = 0; i < nb_ind; i++) {
    ind[i].mutate();
  }
}

void Genetic::genetic(){

  //evalue les population
  old->evaluate_population(image);
  new_->evaluate_population(image);

  //met tous les bon dans old 
  selection();
  //le resultat des crossing des adn de old vont dans new
  crossing_from_population();

  new_->mutate_population();  

}

void Population::flush_population(){
  nb_ind=0;
}

bool Population::population_add(Individu* ind){

  if(nb_ind<size){
    this->ind[nb_ind].L=ind->L;
    this->ind[nb_ind].D=ind->D;
    this->ind[nb_ind].entropy=ind->entropy;
    nb_ind++;
    return true;
  }
  return false;
}

void Population::init(){
  int i;

  for (i = 0; i < 100; i++) {
    ind[i].L=(rand()%16)+1;
    ind[i].D=rand()%(256-ind[i].L);
  }

  nb_ind=100;
}

Population::Population(int size){
  ind=new Individu[size];
  nb_ind=0;
  this->size=size;
}

Individu::Individu():L(0),D(0),entropy(0){
}
