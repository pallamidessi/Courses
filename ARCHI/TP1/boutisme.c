#include "bases.c"
#include <stdlib.h>

/* construit un uint32 à partir de quatre octets */
uint32_t make_uint32_t (uint8_t o0, uint8_t o1, uint8_t o2, uint8_t o3) 
{
    union {uint32_t i; uint8_t o[4];} v;
    v.o[0] = o0; v.o[1] = o1; v.o[2] = o2; v.o[3] = o3; 
    return v.i;
}


/* Main : */
int main(int argc, char** argv)
{
uint32_t resultat;	
uint8_t a=0;
uint8_t b=0;
uint8_t c=0;
uint8_t d=1;

    // faire appel à make_uint32_t et stocker le resultat dans une variable i
		resultat=make_uint32_t(a,b,c,d);
    // Selon la valeur de i, décider du mode	
		if (resultat>1)
			printf("la machine est petit boutiste");
		else 
			printf("la machine est gros boutiste");
	
	return 0;

}

