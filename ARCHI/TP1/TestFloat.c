#include <stdio.h>
#include <stdint.h>



int main (void)
{   

int i ; double S ; S = 1000.1;
printf(" La valeur de S avant le calcul est %f .\n", S);
for (i=0; i<9; i++) S=S+0.1;
printf(" La valeur de S après le calcul est %f .\n", S);


return 0;
}
