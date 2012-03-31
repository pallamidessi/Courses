/**
 * \file       grille.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief       header de grille.c
 *
 * \details    Contient les prototype des fonction relative au logigraphe(grille) ou a leurs traitements.
 *
 */

#include "grille.h"
#include"Image.h"


int Seuil(char* nom_du_fichier){
int i=0;
FILE* fichier = NULL;
int sizeImageH=0,int sizeImageV=0;
char R=0,V=0,B=0;
int total=0,seuil=0;
int ValMax=0;

fichier = fopen(nom_du_fichier, "r");

  if (fichier != NULL)
  {
    fgets(mode,TAILLE_MAX,fichier);
    fgets(chaine,TAILLE_MAX,fichier);
    fscanf(fichier,"%s %s", &sizeImageH,&sizeImageV);
    fscanf(fichier,"%d", &ValMax);

     for(i=0;i<(sizeImageV*sizeImageH);i++){
                 fscanf(fichier,"%c", &R);
                 fscanf(fichier,"%c", &V);
                 fscanf(fichier,"%c", &B);
            total+=R*0.299+V*0.587+B*0.114;
     }
  }
seuil=total/(sizeImageV*sizeImageH);
return seuil;
  }




grille charger_image(grille L, char* nom_du_fichier,char* mode,int seuil){
int i=0,v=0;
char R=0,V=0,B=0;
FILE* fichier = NULL;
int ValMax=0;
char modeImage[2];
int sizeImageH=0,int sizeImageV=0;
char chaine[TAILLE_MAX] = "";

fichier = fopen(nom, mode);

	if (fichier != NULL)
  {

fgets(modeImage,TAILLE_MAX,fichier);             //1er ligne le mode
fgets(chaine,TAILLE_MAX,fichier);                 //2eme ligne meta donnee de creation de l'image -> On passe simplement a la ligne suivante
fscanf(fichier,"%s %s", &sizeImageH,&sizeImageV); //3eme ligne recuperation de la taille de l'image
fscanf(fichier,"%d", &ValMax);                     //4eme valeurs max des composantes

if (sizeImageH>70)                                 // tronque l'image si elle est plus grande que 70 colonneS
    sizeImageH=70;

grille charge=alloue_grille(sizeImageH,sizeImageV);     // creer un logigraphe a la taille de l'image

 if (modeImage=="P3"){

    for(i=0;i<sizeImageV;i++){
        for(v=0;v<sizeImageH;v++){
            fscanf(fichier,"%c", &R);
            fscanf(fichier,"%c", &V);
            fscanf(fichier,"%c", &B);

            if((R*0.299+V*0.587+B*0.114)>=seuil)
                charge[i][v]='+';
            else
                charge[i][v]='.';

        }
    }
 }

  	 fclose(fichier);
  }
return charge;
}
