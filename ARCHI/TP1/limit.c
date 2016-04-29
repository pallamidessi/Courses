#include <stdio.h>

/* Calcul x^n : */
long puiss(int x,int n) { return n==0?1:puiss(x,n-1)*x; }

int main(int argc, char** argv)
{

	unsigned int n=1;
	int m=1;
	int compteur=0;
	printf("entier non signe\n");

	while(n!=0){
		n*=2;
		compteur++;
	}
	printf("on peut stocker %ld sur un entier non signe\n",puiss(2,compteur)-1);

	printf("entier signe\n");

	compteur=0;
	while(m!=0){
		m*=2;
		compteur++;
	}
	printf("on peut stocker %ld sur un entier signe\n",puiss(2,compteur)-1);
return 0;

}
