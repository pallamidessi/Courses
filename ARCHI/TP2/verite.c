#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/******************************************************************************/
// Fonctions F1 et F2
/******************************************************************************/
int F1(int a, int b, int c) { 
	return ((!a&&!b&&c)||
					(!a&&b&&!c)||
					(a&&!b&&!c)||
					(a&&!b&&c)||
					(a&&b&&!c)); 
}

int F2(int a, int b, int c) { 
	return ((a&&!b)||
					(b&&!c)||
					(!b&&c));
}

int F3(int a, int b, int c) { 
	return ((a)||
					(b&&!c)||
					(!a&&!(b&&!c))&&
					((a)||(b))); 
}

int F4(int a, int b, int c) { 
	return a;
}

/******************************************************************************/
// Fonctions travaillant sur la table :
/******************************************************************************/

// Initialisation de la matrice. 4eme colonne à 0 pour l'instant.
void Initialisation_tableverite(int table[8][4])
{
	int i;
	for(i=0;i<8;i++){
		table[i][1]=0;
		table[i][2]=0;
		table[i][3]=0;
	}
	
	for(i=4;i<8;i++){
		table[i][1]=1;
	}
		table[2][2]=1;
		table[3][2]=1;
		table[6][2]=1;
		table[7][2]=1;

		table[1][3]=1;
		table[3][3]=1;
		table[5][3]=1;
		table[7][3]=1;
}

// Calcul de de la 4eme colonne : F1.
void Calcul_tableveriteF1(int table[8][4])
{ // Utilisez les focntion F1 le remplissage de la matrice.
int i;
for(i=0;i<8;i++)
	table[i][4]=F1(table[i][1],table[i][2],table[i][3]);
}

// Calcul de de la 4eme colonne : F2.
void Calcul_tableveriteF2(int table[8][4])
{ // Utilisez les focntion F2 le remplissage de la matrice.
int i;
for(i=0;i<8;i++)
	table[i][4]=F1(table[i][1],table[i][2],table[i][3]);

}
void Calcul_tableveriteF3(int table[8][4])
{ // Utilisez les focntion F2 le remplissage de la matrice.
int i;
for(i=0;i<8;i++)
	table[i][4]=F3(table[i][1],table[i][2],table[i][3]);

}

void Calcul_tableveriteF4(int table[8][4])
{ // Utilisez les focntion F2 le remplissage de la matrice.
int i;
for(i=0;i<8;i++)
	table[i][4]=F4(table[i][1],table[i][2],table[i][3]);
}

// Affichage de la matrice :
void Affichage_tableverite(int table[8][4])
{
int i;
printf(" A B C F1(ou F2)");
for(i=0;i<8;i++)
	printf("|%d|%d|%d|%d|\n",table[i][1],table[i][2],table[i][3],table[i][4]);

}


// Affiche la forme normale disjonctive :
void disjonctive(int table[8][4])
{
	int plus=0;
	int i;
    // Compter le nombre d'éléments à 1 pour le Bon affichage des " + " :
		for(i=1;i<9;i++){
			if (table[i][4]==1)
				plus++;
		}
		plus--;

    // Pour chaque 1 de la colonne 4 .......... Affichage de la ligne suivie du + 
    //	(il faut éviter de terminer la forme par un +)

		for(i=1;i<9;i++){
			if (table[i][4]==1){
				
				if (table[i][1]==1)
					printf("(A");
				else 
					printf("(!A");

				if (table[i][2]==1)
					printf("B");
				else 
					printf("!B");
				
				if (table[i][3]==1)
					printf("C)");
				else 
					printf("!C)");
				
				if(plus>0){
					printf("+");
					plus--;
				}
				}
		}

	printf("\n");
}

// Affiche la forme normale conjonctive :
void conjonctive(int table[8][4])
{
	int i;
	int mult=0;

		for(i=1;i<9;i++){
			if (table[i][4]==0)
				mult++;
		}
		mult--;
		
		for(i=1;i<9;i++){
			if (table[i][4]==0){
				
				if (table[i][1]==0)
					printf("(A+");
				else 
					printf("(!A+");

				if (table[i][2]==0)
					printf("B+");
				else 
					printf("!B+");
				
				if (table[i][3]==0)
					printf("C)");
				else 
					printf("!C)");
				
				if(mult>0){
					printf("*");
					mult--;
				}
				}
		}

	printf("\n");
}

	// Même principe en considérant que les 0 de la colonne 4. 
        // Faîte attention à bien inverser la valeur des colonnes 1 à 3 pour Avoir le produit des sommes.





/******************************************************************************/
// Main :
/******************************************************************************/

int main(int argc, char* argv[])
{
    // Matrice pour table de vérité à 3 variables + 1 résultat :
    int table[8][4];
    

    // Début :
    printf("Initialisation ...... \n");
    Initialisation_tableverite(table);

 
    printf("Remplissage et Affichage / F1......;........ \n");
    Calcul_tableveriteF1(table);
    Affichage_tableverite(table);    
    printf("Forme disjonctive :\n");
    disjonctive(table);
    printf("Forme conjonctive :\n");
    conjonctive(table);


    printf("Remplissage et Affichage / F2......;........ \n");
    Initialisation_tableverite(table);
    Calcul_tableveriteF2(table);
    Affichage_tableverite(table);    
    printf("Forme disjonctive :\n");
    disjonctive(table);
    printf("Forme conjonctive :\n");
    conjonctive(table);

    Initialisation_tableverite(table);
    Calcul_tableveriteF3(table);
    Affichage_tableverite(table);    
    Initialisation_tableverite(table);
    Calcul_tableveriteF4(table);
    Affichage_tableverite(table);    
    
    // Affichage Énigme :
    printf("\nEnigme :\n");
    // .............................;

 
    return 0;
}
