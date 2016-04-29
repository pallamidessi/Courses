#ifndef CLUSTER_H__
#define CLUSTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct strPixel{
  unsigned char R;
  unsigned char G;
  unsigned char B;
  unsigned int X;
  unsigned int Y;
} pixel,*Pixel;

typedef struct strCluster{
  unsigned int size;
  unsigned int nbPixel;
  pixel* list;
  pixel kernel;
} cluster,*Cluster;


#endif /* end of include guard: CLUSTER_H__ */
