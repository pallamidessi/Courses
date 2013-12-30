#include <stdio.h>
#include <stdlib.h>
#include "entropy.h"
#include "bmpfile.h"



int main(int argc, char* argv[]){
  bmpgrey_t* image=simple_import(argv[1]);
  simple_export(image,"test.bmp");
  
  printf("log2(47)=%f\n",log2(47));
  return 0;
}
