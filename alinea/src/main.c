#include"tp1.h"
#include"determinant.h"
#include"gauss.h"
#include"inversion.h"
#include"resol.h"
#include<time.h>
#include<unistd.h>
#include<string.h>
int main(int argc,char** argv){

	char* tvalue=NULL;
	int taille=3;
	srand(time(NULL));
	char command[10];
	char c;
	int det=0;
	int premiere_affi=1;

	bool mode_script=false;
	bool deter_pivot=false;
	bool deter_recur=false;
	bool inver_pivot=false;
	bool inver_comatrice=false;

	while((c=getopt(argc,argv,"sprict:"))!=-1){
			switch(c)
			{
			case 's':
				mode_script=true;
				break;
			case 'p':
				deter_pivot=true;
				break;
			case 'r':
				deter_recur=true;
				break;
			case 't':
				tvalue=optarg;
				break;
			case '?':
				printf("usage: -s mode pour script  -p:determinant par pivot de gauss -r:determinant par algorithme recursif -i:inversion par pivot de gauss -c:inversion par comatrice -t:taille de la matrice\n");
			 default:
				break;
			}
		}

	if(mode_script==true){
		if(tvalue!=NULL)
			taille=atoi(tvalue);

		Matrix m=newMatrix(taille,taille);
		remplissage(m);
	

		if(deter_pivot==true)
			det=m_determinant(m);			
		else if(deter_recur==true)
			det=Determinant(m);
		else if(inver_pivot==true)
			m=inversion_gauss(m);
		else if(inver_comatrice==true)
			m=inversion_comatrice(m);

		deleteMatrix(m);
		return 0;
	}

	printf("On fait un jeu complet de modification \n\n");
	Matrix test=newMatrix(3,3);
	Matrix test2=newMatrix(3,3);	

	remplissage(test);
	remplissage(test2);
	
	
	printf("Matrice A:\n");
	affichage(test);
	printf("\n");

	printf("Matrice B:\n");
	affichage(test2);
	printf("\n\n");

	printf("Addition de A et B\n");
	Matrix add=addition(test,test2);
	
	affichage(add);
	printf("\n\n");
	deleteMatrix(add);

	printf("Multiplication de A et B\n");
	Matrix mult=multiplication(test,test2);
	affichage(mult);
	printf("\n\n");
	deleteMatrix(add);

	printf("Determinant de A par calcul recursif\n");
	det=Determinant(test);
	printf("%d\n",det);
	printf("\n");

	printf("Determinant de A par la methode optimisee du pivot de Gauss\n");

	det=m_determinant(test);
	printf("%d\n",det);
	printf("\n");

	printf("inversion par pivot de gauss\n");
	Matrix copy=copie(test);	
	test=inversion_gauss(test);
	affichage(test2);
	printf("\n");

	printf("inversion par comatrice\n");
	copy=inversion_comatrice(copy);
	affichage(test2);
	printf("\n");

	printf("Inversion par comatrice optimisee\n");


	deleteMatrix(copy);
	deleteMatrix(test);
	deleteMatrix(test2);

	printf("Un systeme s:\n");
	Systeme s=newSystem(3,3);
	remplissage_systeme(s);
	affichage_systeme(s);

	printf("Resolution du systeme s\n");
	printf("\n");
	s=resolution(s);
	affichage_systeme(s);
	printf("\n");
	deleteSysteme(s);


	printf("\n\nListe des commandes possible\n");
	printf("Addition : add \n");
	printf("Multiplication: mult \n");
	printf("Determinant (recursif): deter_recur \n");
	printf("Determinant (pivot de gauss): deter_pivot \n");
	printf("Inversion par pivot:  inver_pivot \n");
	printf("Inversion par comatrice: inver_comatrice \n");
	printf("Inversion par comatrice optimisee: inver_comatrice_op\n");
	printf("Resolution de systeme: resol_systeme \n");
	printf("Decomposition PLU: PLU\n");
	printf("valeur propre: valeur_propre\n\n");


	printf("\nCommande : ");
	while((strcmp(command,"quit"))!=0){

		if(premiere_affi==1)
			premiere_affi=0;
		else
			printf("\nCommande : ");

		scanf("%s",command);

		if (strcmp(command,"add")==0){
	
		}
		else if (strcmp(command,"mult")==0){
		
		}
		else if (strcmp(command,"deter_recur")==0){
		
		}
		else if (strcmp(command,"deter_pivot")==0){
			
		}
		else if (strcmp(command,"inver_pivot")==0){
		
		}
		else if (strcmp(command,"inver_comatrice")==0){
	
		}
		else if (strcmp(command,"inver_comatrice_op")==0){
		
		}
		else if (strcmp(command,"resol_systeme")==0){
			
		}
		else if (strcmp(command,"PLU")==0){
		
		}
		else if (strcmp(command,"valeur_propre")==0){
			
		}
	}

	return 0;	
}
 
