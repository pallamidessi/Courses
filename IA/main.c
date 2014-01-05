#include <cstdio>
#include <cstdlib>
#include "entropy.h"
#include "genetic.h"
#include <ctime>


int main(int argc, char* argv[]){
  Bmpgrey* image=new Bmpgrey(argv[1]);

  int i;
  srand(time(NULL));
  image->create_histo_tab();
  
  for (i = 0; i < 256; i++) {
    printf("%f \n",image->histogram[i]);
  }

  printf("log2(0.416672)=%f\n",Bmpgrey::log2(0.416672));
  
  double ent=image->entropy(256,0);

  printf("%f\n",ent);

  //Individu* best=image->color_reduction_4bit();
  //printf("%d %d %f\n",best->L,best->D,best->entropy);
  
  Population* old=new Population(100);
  Population* new_=new Population(100);

  old->init();
  new_->init();
  
  Genetic* gen=new Genetic(old,new_,image);

  printf("Debut genetic\n");
  for (i = 0; i < 300; i++) {
    gen->genetic();
    printf("Generation %d\n",i);
  }

  printf("L %d D %d entropie:%f\n",old->ind[0].L,old->ind[0].D,old->ind[0].entropy);

  
  return 0;
}
