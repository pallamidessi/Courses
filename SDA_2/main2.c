#include"arbin2.h"

int main(){

arbin a=arbre_nouv();

a=enracinement(enracinement(NULL,"15",NULL),"*",enracinement(NULL,"10",NULL));
affiche_terme(a);
printf("\n%d",evaluer(a));
return 0;
	
}
