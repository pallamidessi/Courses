#include <stdio.h>
#include <stdint.h>

/* retourne le n-i√®me (0 <= n <= 31) bit d'un float */
int nieme_bit(float f, int n)
{
    union {uint32_t i; float f;} v;
    v.f = f;
    return (v.i >> n) & 0x1;
}



int main (void)
{   
int i;
float a;
 /* On demande un float */
		printf("Rentrez un float:\n");
		scanf("%f",&a);
    /* Extraction du signe, ... */
		
		printf("Le Signe\n");
		printf("%d\n",nieme_bit(a,31));
    /* ..., de l'exposant ‡ partir de l'exposant biaisÈ, ... */
   	 
		printf("L'exposant biaisee\n");
		
		for(i=30;i>=22;i--)
			printf("%d",nieme_bit(a,i));
			
		printf("\n");

    /* ... et de la mantisse */
		printf("La mantisse\n");
		
		for(i=23;i>=0;i--)
			printf("%d",nieme_bit(a,i));

			printf("\n");
    /* Affichage du nombre dÈcortiquÈ */
    
		printf("Le nombre decortique\n");
		
		for(i=31;i>=0;i--)
			printf("%d",nieme_bit(a,i));

			printf("\n");

    return 0;
}

