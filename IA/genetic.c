#include "genetic.h"

/*Genetic methods ----------------------------------------------------------------*/



/*Constructor of Genetics*/
Genetic::Genetic(Population* old,Population* new_,Bmpgrey* image){
  this->old=old;
  this->new_=new_;
  this->image=image;
}



/*Crossing over between two DNA*/
Individu* Genetic:: crossing_over(Individu* A,Individu* B){
  static Individu* C=NULL;

  if(C==NULL)
    C=new Individu();

  C->L=A->L;
  C->D=B->D;
  C->entropy=0.00;

  return C;
}


/*Crossoving over between DNA in Popuulation old. Newly create DNA are placed in
 * Population new_ .
 * Choisi aleatoirement les individus de old (les bon de la generation precedente) et les
cross entre eux et les place dans new, old est trie par la selection*/
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


/*Select the best DNA in the two given population, and put them in populuation old*/
void Genetic::selection(){
  static Population* selected=NULL;
  int i;
  
  /*Alloc only once this temporary Population which is used as container*/
  if(selected==NULL)
    selected=new Population(POPULATION_SIZE*2);

  selected->flush_population();
  Quicksort::quicksort_population(old,new_,selected);

  //old contient maintenant tout les bon individu
  for(i=0;i<POPULATION_SIZE;i++)
    old->ind[i]=selected->ind[(POPULATION_SIZE*2-1)-i];
}


/*Wrapper for all the functions of the genetic algorithm*/
void Genetic::genetic(){

  //Evalue les populations(entropie selon chaque individu)
  old->evaluate_population(image);
  new_->evaluate_population(image);

  //Met tous les bon dans old 
  selection();
  //Le resultat des crossing des adn de old vont dans new
  crossing_from_population();
  
  //Mutation a hauteur de 4%
  new_->mutate_population();  
}


/*Population methods ----------------------------------------------------------------*/


/*Constructor for Population*/
Population::Population(int size){
  ind=new Individu[size];
  nb_ind=0;
  this->size=size;
}


/*Reset the DNA count in a population*/
void Population::flush_population(){
  nb_ind=0;
}


/*Add a DNA in the population, if possible*/
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


/*Create random values for each DNA in a population*/
void Population::init(){
  int i;

  for (i = 0; i < POPULATION_SIZE; i++) {
    ind[i].L=(rand()%16)+1;
    ind[i].D=rand()%(256-ind[i].L);
  }

  nb_ind=POPULATION_SIZE;
}


/*Try to mutate each DNA in a population*/
void Population::mutate_population(){
  int i;
 
  for (i = 0; i < nb_ind; i++) {
    ind[i].mutate();
  }
}


/*Calculate entropy for each DNA for a given greyscale bmp image*/
void Population::evaluate_population(Bmpgrey* image){
  int i;

  for (i = 0; i < nb_ind; i++) {
    ind[i].evaluate(image);;
  }
}


/*individu methods ----------------------------------------------------------------*/


/*Constructor for Individu (DNA)*/
Individu::Individu():L(0),D(0),entropy(0){
}


/*4% chance to mutate an Individu using specific bounds*/
void Individu::mutate(){
  int mut_chance;
  int threshold1=4;
  int threshold2=2;
  int signe;

  mut_chance=rand()%100;

  /*Mutation de la longueur de la fenetre*/
  if (mut_chance<threshold2) {
    signe=rand()%2;
    if (signe==0 && L>0) {
      L--;
    }
    else if(L<15){
      L++;
    }
  }
  /*Mutation du decalage de la fenetre*/
  else if(mut_chance<threshold1){
    signe=rand()%2;
    if (signe==0 && D>5) {
      D-=5;
    }
    else if(D<250)
      D+=5;
  }

}


/*Calculate entropy of the given image using the DNA as parameter*/
void Individu::evaluate(Bmpgrey* image){
  entropy=image->entropy(L,D);
}

