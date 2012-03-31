

char* ModePPM(char* nom_du_fichier){

char szMode[2];
FILE* fichier = NULL;
fichier = fopen(nom_du_fichier,"r")

	if (fichier != NULL)
  {
fgets(chaine, TAILLE_MAX, fichier);
               szMode=chaine;
  }

  return szMode;
}

int* SizePPM(char* nom_du_fichier){
int sizeHV[1];
FILE* fichier = NULL;
fichier = fopen(nom_du_fichier,"r")

	if (fichier != NULL)
  {
fgets(chaine, TAILLE_MAX, fichier);
               szMode=chaine;
  }

  return szMode;
}


grille charger_grille(grille L, char* nom,char* mode){
int i=0,v=0;
grille charge=alloue_grille(L->N,L->M);
FILE* fichier = NULL;

char sizeImage[100];
int sizeImageH;
int sizeImageV;
char chaine[TAILLE_MAX] = "";

fichier = fopen(nom, mode);

	if (fichier != NULL)
  {


          fgets(chaine, TAILLE_MAX, fichier);
            if chaine=="P3"
               szMode=chaine;

            fgets(chaine, TAILLE_MAX, fichier);
            sizeImage=chaine

            while(chaine[i]!='\0'){

            }
      }

 		 while (fgets(chaine, TAILLE_MAX, fichier) != NULL )
     {
     	 for(i=0;i<L->M;i++)
       charge->matrice[v][i]=chaine[i];
			 v++;
	   }

  	 fclose(fichier);
  }
return charge;
}
