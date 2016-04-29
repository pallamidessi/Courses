#include"arbin.h"

int main(){

arbin a=arbre_nouv();

a=enracinement(enracinement(NULL,15,NULL),45,enracinement(NULL,10,NULL));
affiche(a);
return 0;
	
}
