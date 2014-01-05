#include "entropy.h"

Bmpgrey::Bmpgrey(char* filename){
  FILE* file =NULL; 
  int length=1,height=1,depth=1;
  int i;
  int px_total=0;
  unsigned int offset=0;
  int current_grey=0;
  int nb_color;

  file=fopen(filename, "rb");
  /*Recuperation de la longueur et de la largeur*/
  fseek(file,18,SEEK_SET);
  fread(&length,sizeof(int),1,file);
  fread(&height,sizeof(int),1,file);

  /*Recuperation de la profondeur des couleurs*/
  fseek(file,2,SEEK_CUR);
  fread(&depth,sizeof(char),2,file);

  /*Allocation de la structure contenant l'image */
  greyscale=new int[length*height];

  /*Recuperation de l'offset contenant le debut du PixelArray*/
  fseek(file,10,SEEK_SET);
  fread(&offset,sizeof(int),1,file);

  /*Recuparation des pixel (niveau de gris)*/
  fseek(file,offset,SEEK_SET);
  px_total=height*length;

  for (i = 0; i < px_total; i++) {
    fread(&current_grey,1,1,file);
    greyscale[i]=current_grey;
    current_grey=0;
  }
  fclose(file);
  
  nb_color=(int) pow(2,depth);
  
  this->color_depth=depth;
  this->length=length;
  this->height=height;
  this->nb_color=nb_color;
  this->histogram=new double[nb_color];
}

/*Logarithme en base 2*/
 double Bmpgrey::log2(double x){
  return log(x)/log(2);
}
/*
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
			rgb_pixel_t pixel = {coul, coul, coul, 0};
			bmp_set_pixel(bmp, j, i, pixel);
      coul=0;
		}
	}
	bmp_save(bmp, bmpname);
	bmp_destroy(bmp);
}
*/

/*Creation et calcul de l'histogramme  */
void Bmpgrey::create_histo_tab(){
  int i;
  int px_total=height*length;
  
  for (i = 0; i < nb_color; i++) {
    histogram[i]=0;
  }

  /*On compte chaque occurence de couleur dans l'image*/
  for (i = 0; i < px_total; i++) {
    histogram[greyscale[i]]+=1;
  }

}

/*Calcul de l'entropie au sens de Shannon d'une image*/
double Bmpgrey::entropy(int width_window,int decal_window){
  int i;
  int current_grey;
  double entropy=0.00000;
  double* histo=this->histogram;
  int px_total=height*length;
  double val=1;
  int count=0;
  
  for (i = decal_window; i < width_window+decal_window; i++) {
    count+=histo[i];
  }
  
  if (count==0) {
    return 0.0;
  }

  for (i = 0; i < px_total; i++) {
    current_grey=this->greyscale[i];
    
    if (current_grey>=decal_window && current_grey<(width_window+decal_window)) {
      val=histo[current_grey]/count;
      entropy+=val*log2(val);
    }
  }

  entropy*=-1;
  entropy/=count;
  return entropy;
}

/*Reduction de couleur en moyennant */
Individu* Bmpgrey:: color_reduction_4bit(){
  double best_entropy,tmp_entropy;
  int first=1;
  int L,D;
  
  Individu* best_ind=new Individu();


  for (L = 16; L > 0; L--) {

    for (D = 0; D < 256-256/L; D++) {
      if(first){
        best_entropy=this->entropy(L,D);
        best_ind->D=D;
        best_ind->L=L;
        best_ind->entropy=best_entropy;
        first=0;
      }
      else if(best_entropy<(tmp_entropy=this->entropy(L,D))){
        best_entropy=tmp_entropy;
        best_ind->D=D;
        best_ind->L=L;
        best_ind->entropy=best_entropy;
      }
    }
  }
  printf("best entropy %f\n",best_entropy);
  printf("L %d D %d\n",best_ind->L,best_ind->D);
  return best_ind;
}


