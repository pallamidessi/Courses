#ifndef KMEANS_H__
#define KMEANS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cluster.h"
#include "image.h"
#include <omp.h>


void addPixelCluster(cluster* cl, pixel* px);
pixel randomPixel(pixel** matrix,unsigned int height,unsigned int length);

pixel* initRandom(unsigned int K, pixel** pixelMatrix,
                  unsigned int height,unsigned int length);

cluster* initCluster(unsigned int K);

unsigned int  distancePixel(pixel* px1,pixel* px2);

void segmentation(pixel* clusterKernel,cluster* cluster,unsigned nbCluster,
                  pixel** matrix,unsigned int height, unsigned int length);

void updateKernel(pixel* clusterKernel,cluster* cluster,unsigned int K);

void kmeans(pixel* clusterKernel,cluster* cluster,Image matrix,unsigned K);
#endif /* end of include guard: KMEANS_H__ */
