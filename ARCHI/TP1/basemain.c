#include "bases.c"



int main(int argc, char** argv)
{
    int entier;
    int base;

    printf("Entrer un nombre compris entre 0 et 255 : ");
    scanf("%d",&entier);

    printf("Base 2 : ");
    base2(entier);
    printf("\n");

    printf("Base 8 : ");
    base8(entier);
    printf("\n");

    printf("Base 16 : ");
    base16(entier);
    printf("\n");

    printf("Entrer une base comprise entre 1 et 36 : ");
    scanf("%d",&base);
    printf("Base %d : ",base);
    basen(entier, base);
    printf("\n");

    return 0;
}
