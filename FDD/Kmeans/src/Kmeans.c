#include <stdio.h>
#include <stdlib.h>
#include "kmeans.h"


void addPixelCluster(cluster* cl, pixel* px){
  if(cl->size==cl->nbPixel){
    realloc(cl->list,cl->size*2);
    cl->size*=2;
  }

  cl->list[cl->nbPixel]=*px;
  cl->nbPixel++;
}


pixel randomPixel(pixel** matrix,unsigned int height,unsigned int length){
  int L=rand()%length;
  int H=rand()%height;
  
  return matrix[H][L];
}


pixel* initRandom(unsigned int K, pixel** pixelMatrix,unsigned int height,
                                                      unsigned int length){
  pixel* clusterKernel=malloc(K*sizeof(pixel));
  unsigned int i;

  for (i = 0; i < K; i++) {
    clusterKernel[i]=randomPixel(pixelMatrix,height,length);
  }
  
  return clusterKernel;
}


cluster* initCluster(unsigned int K){
  int i;
  cluster* cl=malloc(K*sizeof(struct strCluster));
   
  for (i = 0; i < K; i++) {
    cl[i].list=malloc(pow(2,16)*sizeof(pixel));
    cl[i].size=pow(2,16);
    cl[i].nbPixel=0;
  }
  return cl;
}


unsigned int  distancePixel(pixel* px1,pixel* px2){
 return sqrt(((px1->R - px2->R)*(px1->R - px2->R))+
         ((px1->G - px2->G)*(px1->G - px2->G))+
         ((px1->B - px2->B)*(px1->B - px2->B)));
}


void segmentation(pixel* clusterKernel,cluster* cluster,unsigned nbCluster,
                  pixel** matrix,unsigned int height, unsigned int length){
  int i,j,k;
  int tmpDistance=-1;
  int minDistance=-1;
  int minCluster=-1;
  
  #pragma omp parallel for 
  for (i = 0; i < height; i++) {
    for (j = 0; j < length; j++) {
      for (k= 0; k < nbCluster; k++) {

        tmpDistance=distancePixel(&matrix[i][j],&clusterKernel[k]);
        
        if(k==0)
          minDistance=tmpDistance;
        
        if (tmpDistance<minDistance) {
          minCluster=k;  
        }
        addPixelCluster(&cluster[minCluster],&matrix[i][j]);
      }
    }
  }
}


void updateKernel(pixel* clusterKernel,cluster* cluster,unsigned int K){
  int i;
  int R=0;
  int G=0;
  int B=0;
  int size=cluster[K].size;

  for (i = 0; i < size; i++) {
    R+=cluster[K].list[i].R; 
    G+=cluster[K].list[i].G; 
    B+=cluster[K].list[i].B; 
  }
  
  R/=size;
  G/=size;
  B/=size;

  clusterKernel[K].R=R;
  clusterKernel[K].G=G;
  clusterKernel[K].B=B;

}


void kmeans(pixel* clusterKernel,cluster* cluster,Image toSegment,unsigned K){
  int i,k;

  for (i = 0; i < 10; i++) {
    
    for (k = 0; k < K; k++) {
      cluster[K].nbPixel=0;
    }

    segmentation(clusterKernel,cluster,K,toSegment->matrix,toSegment->height,toSegment->length);
    
    #pragma omp parallel for 
    for (k = 0; k < K; k++) {
      updateKernel(clusterKernel,cluster,k);  
    }
    

  }  

}
