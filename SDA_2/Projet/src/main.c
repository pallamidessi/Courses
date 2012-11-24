#include"arbq1.h"
#include"iobmp.h"
#include<string.h> 

int main(){
	char command[10];
	char filename[30]="strasbourg_matrix";
	Arbq transf;
	bool premiere_affi=1;
	int fin_nom;

	printf("choisir un image carre dans le dossier couranti (sans l'extension): \n");
//	gets(filename);
	fin_nom=strlen(filename);
	
	Arbq arbre = import(strcat(filename,".mat"));
	filename[fin_nom]='\0';
	
	printf("Rotation a gauche: rotg \n");
	printf("Rotation a droite: rotd \n");
	printf("Symetrie horizontal: symh \n");
	printf("Symetrie vertical: symv \n");
	printf("Reduction image: dzoo\n");
	printf("Inversion couleur: invc \n");
	printf("Obscursir selon un seuil: tresh \n");
	printf("Niveau de gris: nivg\n");
	printf("\nCommande : ");
	
	while((strcmp(command,"quit"))!=0){
	
	if(premiere_affi==1)
		premiere_affi=0;
	else
		printf("\nCommande : ");
		
	gets(command);

	if (strcmp(command,"rotg")==0){
		transf=rotg(arbre);
		exportBmp(transf,strcat(filename,"_rotg.bmp"));
		filename[fin_nom]='\0';
	}
	else
		if (strcmp(command,"rotd")==0){
			transf=rotd(arbre);
			exportBmp(transf,strcat(filename,"_rotd.bmp"));
			filename[fin_nom]='\0';
		}
	else
		if (strcmp(command,"symh")==0){
			transf=symh(arbre);
			exportBmp(transf,strcat(filename,"_symh.bmp"));
			filename[fin_nom]='\0';
		}
	else
		if (strcmp(command,"symv")==0){
			transf=symv(arbre);
			exportBmp(transf,strcat(filename,"_symv.bmp"));
			filename[fin_nom]='\0';
		}
	else
		if (strcmp(command,"dzoo")==0){
			transf=dzoo(arbre);
			exportBmp(transf,strcat(filename,"_dzoo.bmp"));
			filename[fin_nom]='\0';
		}
	else
		if (strcmp(command,"invc")==0){
			transf=parc(arbre,invc);
			exportBmp(transf,strcat(filename,"_invc.bmp"));
			filename[fin_nom-1]='\0';
		}
	else
		if (strcmp(command,"tresh")==0){
			//transf=parc(arbre,tresh);
			exportBmp(transf,strcat(filename,"_tresh.bmp"));
			filename[fin_nom-1]='\0';
		}
	else
		if (strcmp(command,"nivg")==0){
			transf=parc(arbre,nivg);
			exportBmp(transf,strcat(filename,"_nivg.bmp"));
			filename[fin_nom-1]='\0';
		}
	}

	return 0;
}
