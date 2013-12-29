#include "entropy.h"

struct _bmpfile {
  int  length;
  int  height;
  int color_depth;
  double* histogram;
  int* greyscale;
  int nb_color;
};

typedef struct individu{
  float L;
  int D;
}individu_t;

typedef struct _bmpfile bmpfile_t;

bmpfile_t simple_import(cahr* filename){
  FILE* file = fopen(filename, "r");
  int length,height,depth;
  int i;
  int offset=0;
  int current_grey=0;

  /*Recuperation de la longueur et de la largeur*/
  fseek(file,14,SEEK_SET);
  fscanf(file,"%d",&length);
  fscanf(file,"%d",&heigth);

  /*Recuperation de la profondeur des couleurs*/
  fseek(file,2,SEEK_CUR);
  fscanf(file,"%d",&depth);

  /*Allocation de la structure contenant l'image */
  bmpfile_t* image=malloc(sizeof(struct _bmpfile));
  image->greyscale=calloc(length*heigth,depth);

  /*Recuperation de l'offset contenant le debut du PixelArray*/
  fseek(file,10,SEEK_SET);
  fscanf(file,"%d",&offset);

  /*Recuparation des pixel (niveau de gris)*/
  fseek(file,offset,SEEK_SET);
  for (i = 0; i < height*length; i++) {
    fread(&current_grey,depth,1,file);
    image->greyscale[i]=pixel_grey;
    fseek(file,2*depth,SEEK_CUR);
    current_grey=0;
  }

  image->color_depth=depth;
  image->length=length;
  image->height=height;
}

/*Logarithme en base 2*/
double log2(double x){
  return log(x)/log(10);
}

/*Creation et calcul de l'histogramme  */
bmp_p create_histo_tab(bmpfile_t* image){
  int nb_colour=(int) pow(2,image->depth);
  double* histo=malloc(sizeof(double)*nb_colour);
  int heigth=image->height;
  int length=image->length;

  /*On compte chaque occurence de couleur dans l'image*/
  for (i = 0; i < length*height; i++) {
    histo[image->greyscale[i]]+=1;
  }

  /*on fait la moyenne des apparition de chaque couleurs*/
  for (i = 0; i <nb_colour; i++) {
    histo[i]/=heigth*length;
  }

  bmp_p->histogram=histo;
}

/*Calcul de l'entropie au sens de Shannon d'une image*/
double entropy(bmpfile_t* image,double* histo,int width){
  int i,j;
  int current_grey;
  int length=image->length;
  int height=image->height;
  int nb_colour=image->nb_color;
  int part=nb_color/width;

  for (i = 0; i < height*length; i++) {
    current_grey=image->greyscale[i];
    
    entropy+=histo[current_grey/part]*log2(histo[current_grey/part]);
  }

  entropy*=-1;

  return entropy;
}

/*Reduction de couleur en moyennant */
individu_t* color_reduction_4bit(bmpfile_t image){
  double tmp_histo[16];
  individu_t* best_ind=malloc(sizeof(struct individu));
  double best_entropy;
  int first=1;
  int count;
  for (i = 0; i < 16; i++) {
    tmp_histo=0;
  }

  for (L = 16; L > 0; L--) {
    for (D = 0; D < 256-256/L; D++) {
      for (i = 0; i < L; i++) {
        for (j = i*image->nb_color/L+D; j < (i+1)*image->nb_color/L+D; j++) {
          if (j<256) {
            tmp_histo[i]+=image->histogram[j];
            count++;
          }
        }
      }
      for (i = 0; i < L; i++) {
        tmp_histo[i]/=count;
      }

      if(first){
        best_entropy=entropy(image,tmp_histo,L);
        first=0;
      }

    }
  }
}
