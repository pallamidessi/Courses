#include <stdio.h>
#include <stdlib.h>
#include "entropy.h"
#include "bmpfile.h"
#include "genetic.h"
#include <time.h>


int main(int argc, char* argv[]){
  bmpgrey_t* image=simple_import(argv[1]);
  int i;
  srand(time(NULL));
  create_histo_tab(image);
  
  for (i = 0; i < 256; i++) {
    printf("%f \n",image->histogram[i]);
  }
  printf("log2(0.416672)=%f\n",log2(0.416672));
  
  double ent=entropy(image,256,0);
  printf("%f\n",ent);
  color_reduction_4bit(image);
  simple_export(image,"test.bmp");
  
  population_t* old=create_population(POPULATION_SIZE);
  population_t* new=create_population(POPULATION_SIZE);

  init(old);
  init(new);
  
  printf("Debut genetic\n");
  for (i = 0; i < 300; i++) {
    genetic(image,old,new);
    printf("Generation %d\n",i);
  }

  printf("L %d D %d entropie:%f\n",old->ind[0].L,old->ind[0].D,old->ind[0].entropy);

  
  return 0;
}
