#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cluster.h"
#include "kmeans.h"
#include "image.h"

int main(int argc, char* argv[]){
  unsigned int K= atoi(argv[1]);

  image toSegment=loadImage(argv[2]);
  srand(time(NULL));

  pixel* kernel=initRandom(K,toSegment.matrix,toSegment.height,toSegment.length);
  
  cluster* cluster=initCluster(K);

  kmeans(kernel,cluster,&toSegment,K);
  return 0;
}
