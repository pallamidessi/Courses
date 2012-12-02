#include"arbq1.h"
#include"iobmp.h"
#include<string.h> 

int main(){
	char command[10];
	char filename[]="strasbourg_matrix";
	Arbq transf;
	bool premiere_affi=1;
	int fin_nom;
	int s;

	fin_nom=strlen(filename);

	Arbq arbre = import(strcat(filename,".mat"));
	filename[fin_nom]='\0';

	printf("On fait un jeu complet de modification \n\n");
	
	printf("Rotation a gauche\n");
	transf=rotg(arbre);
	exportBmp(transf,strcat(filename,"_rotg.bmp"));
	filename[fin_nom]='\0';
	
	printf("Rotation a droite\n");
	transf=rotd(arbre);
	exportBmp(transf,strcat(filename,"_rotd.bmp"));
	filename[fin_nom]='\0';
	
	printf("Symetrie horizontal par rapport au milieu\n");
	transf=symh(arbre);
	exportBmp(transf,strcat(filename,"_symh.bmp"));
	filename[fin_nom]='\0';
	
	printf("Symetrie vertical par rapport au milieu\n");
	transf=symv(arbre);
	exportBmp(transf,strcat(filename,"_symv.bmp"));
	filename[fin_nom]='\0';
	
	printf("Symetrie horizontal\n");
	transf=symhT(arbre);
	exportBmp(transf,strcat(filename,"_symhT.bmp"));
	filename[fin_nom]='\0';
	
	printf("Symetrie vertical\n");
	transf=symvT(arbre);
	exportBmp(transf,strcat(filename,"_symvT.bmp"));
	filename[fin_nom]='\0';
	
	printf("Reduction image\n");
	transf=dzoo(arbre);
	exportBmp(transf,strcat(filename,"_dzoo.bmp"));
	filename[fin_nom]='\0';
	
	printf("Inversion couleur\n");
	transf=parc(arbre,invc);
	exportBmp(transf,strcat(filename,"_invc.bmp"));
	filename[fin_nom]='\0';
	
	printf("Obscursir selon un seuil:150\n");
	transf=parc1(arbre,tresh,150);
	exportBmp(transf,strcat(filename,"_tresh.bmp"));
	filename[fin_nom]='\0';
	
	printf("Niveau de gris\n");
	transf=parc(arbre,nivg);
	exportBmp(transf,strcat(filename,"_nivg.bmp"));
	filename[fin_nom]='\0';

	
	printf("\n\nModification multiple sur une seule image\n");
	printf("Rotation a gauche: rotg \n");
	printf("Rotation a droite: rotd \n");
	printf("Symetrie horizontal par rapport au milieu: symh \n");
	printf("Symetrie vertical par rapport au milieu: symv \n");
	printf("Symetrie horizontal: symhT \n");
	printf("Symetrie vertical : symvT \n");
	printf("Reduction image: dzoo\n");
	printf("Inversion couleur: invc \n");
	printf("Obscursir selon un seuil: tresh \n");
	printf("Niveau de gris: nivg\n\n");


	transf=arbre;
	printf("\nCommande : ");
	while((strcmp(command,"quit"))!=0){

		if(premiere_affi==1)
			premiere_affi=0;
		else
			printf("\nCommande : ");

		scanf("%s",command);

		if (strcmp(command,"rotg")==0){
			transf=rotg(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"rotd")==0){
			transf=rotd(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"symh")==0){
			transf=symh(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"symv")==0){
			transf=symv(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"symhT")==0){
			transf=symhT(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"symvT")==0){
			transf=symvT(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"dzoo")==0){
			transf=dzoo(transf);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"invc")==0){
			transf=parc(transf,invc);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"tresh")==0){
			printf("\nseuil : ");
			scanf("%d",&s);
			transf=parc1(transf,tresh,s);
			filename[fin_nom]='\0';
		}
		else if (strcmp(command,"nivg")==0){
			transf=parc(transf,nivg);
			filename[fin_nom]='\0';
		}
	}
	exportBmp(transf,strcat(filename,"_perso.bmp"));

	detruire_Arbq(arbre);
	return 0;
}
