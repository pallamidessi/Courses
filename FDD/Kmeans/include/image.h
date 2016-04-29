#ifndef IMAGE_H__
#define IMAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include "cluster.h"
#include <omp.h>

typedef struct strImage {
  pixel** matrix;
  unsigned height;
  unsigned length;

} image,*Image;

image loadImage(char* path);

#endif /* end of include guard: IMAGE_H__ */
