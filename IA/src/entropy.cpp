#include "entropy.h"

/*Bmpgrey methods---------------------------------------------------*/


/*Constructor of Bmpgrey*/
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



/*Calcul de l'entropie au sens de Shannon d'une image suivant une taille et un decalage de
 * fenetre précisé*/
double Bmpgrey::entropy(int width_window,int decal_window){
  int i;
  int current_grey;
  double entropy=0.00000;
  double* histo=this->histogram;
  int px_total=height*length;
  double val=1;
  int count=0;
  
  /*Compte le nombre effectif d'element pris en compte par le couple L et D*/
  for (i = decal_window; i < width_window+decal_window; i++) {
    count+=histo[i];
  }
  
  /*Pas d'effectif == entropie nulle*/
  if (count==0) {
    return 0.0;
  }

  /*Calcul de l'entropie a proprement dit*/
  for (i = 0; i < px_total; i++) {
    current_grey=this->greyscale[i];
    
    if (current_grey>=decal_window && current_grey<(width_window+decal_window)) {
      val=histo[current_grey]/count;
      entropy+=val*log2(val);
    }
  }

  entropy*=-1;
  /*On normalise par l'effectif : metric entropy*/
  entropy/=count;

  return entropy;
}


/*Recherche exhaustive du meilleur couple L/D */
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

  return best_ind;
}


