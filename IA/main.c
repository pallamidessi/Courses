#include <cstdio>
#include <cstdlib>
#include "entropy.h"
#include "genetic.h"
#include <ctime>


int main(int argc, char* argv[]){
  double ent;
  int i;
  /*Importe l'image*/
  Bmpgrey* image=new Bmpgrey(argv[1]);

  srand(time(NULL));

  /*Creer l'histogramme de l'image*/
  image->create_histo_tab(); 
  
  /*Calcul de l'entropie de l'image d'origine*/
  ent=image->entropy(256,0);
  printf("Entropie de l'image d'origine%f\n",ent);
  printf("\n");
  
  /*Recherche exhaustive du meilleur couple decalage/taille de fenetre*/
  printf("Recherche du meilleurs couple de manière exhaustive\n");
  Individu* best=image->color_reduction_4bit();
  printf("%d %d %f\n",best->L,best->D,best->entropy);
  printf("\n");
  

  /*Creation et initialisation des Populations de depart*/
  Population* old=new Population(POPULATION_SIZE);
  Population* new_=new Population(POPULATION_SIZE);

  old->init();
  new_->init();
  
  /*deroulement de l'algo genetique sur n generation*/
  Genetic* gen=new Genetic(old,new_,image);
  printf("Debut de l'algo genetique \n");
  for (i = 0; i < 100; i++) {
    gen->genetic();
    printf("Generation %d\n",i);
  }

  printf("Meilleurs individu au bout de %d génération:L %d D %d entropie:%f\n",i,old->ind[0].L,old->ind[0].D,old->ind[0].entropy);

  
  return 0;
}
