#include "entropy.h"



bmpgrey_t* simple_import(char* filename){
  FILE* file =NULL; 
  int length=1,height=1,depth=1;
  int i;
  unsigned int offset=0;
  int current_grey=0;

  file=fopen(filename, "rb");
  /*Recuperation de la longueur et de la largeur*/
  fseek(file,18,SEEK_SET);
  fread(&length,sizeof(int),1,file);
  fread(&height,sizeof(int),1,file);

  /*Recuperation de la profondeur des couleurs*/
  fseek(file,2,SEEK_CUR);
  fread(&depth,sizeof(char),2,file);

  /*Allocation de la structure contenant l'image */
  bmpgrey_t* image=malloc(sizeof(struct _bmpfile2));
  image->greyscale=calloc(length*height,sizeof(int));

  /*Recuperation de l'offset contenant le debut du PixelArray*/
  fseek(file,10,SEEK_SET);
  fread(&offset,sizeof(int),1,file);

  /*Recuparation des pixel (niveau de gris)*/
  fseek(file,offset,SEEK_SET);
  for (i = 0; i < height*length; i++) {
    fread(&current_grey,1,1,file);
    //printf("%d %d\n",current_grey,i);
    image->greyscale[i]=current_grey;
    //fseek(file,1,SEEK_CUR);
    current_grey=0;
  }
  
  fclose(file);
  image->color_depth=depth;
  image->length=length;
  image->height=height;

  return image;
}

/*Logarithme en base 2*/
double log2(double x){
  return log(x)/log(10);
}

void simple_export(bmpgrey_t* image, char* bmpname){
	int i,j;
	unsigned char coul=0;
  int length=image->length;
  int height=image->height;

  bmpfile_t* bmp = bmp_create(length,height, 8);

  for (i = 0; i < height; i++)
	{
		for (j = 0; j < length; j++)
		{
			coul =(char)image->greyscale[i*length+j];
      printf("%d \n",coul);
			rgb_pixel_t pixel = {coul, coul, coul, 0};
			bmp_set_pixel(bmp, j, i, pixel);
      coul=0;
		}
	}
	bmp_save(bmp, bmpname);
	bmp_destroy(bmp);
}


/*Creation et calcul de l'histogramme  */
void create_histo_tab(bmpgrey_t* image){
  int nb_color=(int) pow(2,image->color_depth);
  double* histo=malloc(sizeof(double)*nb_color);
  int height=image->height;
  int length=image->length;
  int i;

  /*On compte chaque occurence de couleur dans l'image*/
  for (i = 0; i < length*height; i++) {
    histo[image->greyscale[i]]+=1;
  }

  /*on fait la moyenne des apparition de chaque couleurs*/
  //for (i = 0; i <nb_color; i++) {
  //  histo[i]/=height*length;
  //}

  image->histogram=histo;
  image->nb_color=nb_color;
}

/*Calcul de l'entropie au sens de Shannon d'une image*/
double entropy(bmpgrey_t* image,double* histo,int width){
  int i;
  int current_grey;
  int length=image->length;
  int height=image->height;
  int nb_color=image->nb_color;
  int part=nb_color/width;
  double entropy=0.0;

  for (i = 0; i < height*length; i++) {
    current_grey=image->greyscale[i];
    
    entropy+=(histo[current_grey/part]*log2(histo[current_grey/part]));
  }

  entropy*=-1;

  return entropy;
}

/*Reduction de couleur en moyennant */
individu_t* color_reduction_4bit(bmpgrey_t* image){
  double tmp_histo[16];
  double best_entropy,tmp_entropy;
  int first=1;
  int count;
  int L,D;
  int i,j;
  
  individu_t* best_ind=malloc(sizeof(struct individu));

  /*init histo temporaire*/
  for (i = 0; i < 16; i++) {
    tmp_histo[i]=0;
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
        best_ind->D=D;
        best_ind->L=L;
        first=0;
      }
      else if(best_entropy<(tmp_entropy=entropy(image,tmp_histo,L))){
        best_entropy=tmp_entropy;
        best_ind->D=D;
        best_ind->L=L;
      }
    }
  }

  return best_ind;
}
