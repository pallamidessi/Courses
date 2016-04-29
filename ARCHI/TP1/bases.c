#include <stdio.h>      // Entrée / Sortie standard
#include <stdint.h>     // Entiers standard


int d2c (int n)
{
    if(0<=n && n<10)
        return '0'+n;
    else    if (n < 36)
        return 'A'+ (n-10);
    else return '?';
}


void base2(uint8_t entier)
{
int i;
for(i=128;i>0;i/=2){
	printf("%d",(entier/i)%2);
}
}

void base8(uint8_t entier)
{
int i;
int table[8];	

for(i=1;i<=8;i++){
	table[i-1]=(entier%8);
  entier/=8;
}
for(i=7;i>=0;i--)
	printf("%d",table[i]);

}
void base16(uint8_t entier)
{

int i; 
int table[8];	

for(i=1;i<=8;i++){
	table[i-1]=(entier%16);
  entier/=16;
}
for(i=7;i>=0;i--){
	if(table[i]>9)
		printf("%c",d2c(table[i]));
	else
	printf("%d",table[i]);
}
}

void basen(uint8_t entier, uint8_t base)
{
int i; 
int table[8];	

for(i=1;i<=8;i++){
	table[i-1]=(entier%base);
  entier/=base;
}
for(i=7;i>=0;i--){
	if(table[i]>9)
		printf("%c",d2c(table[i]));
	else
	printf("%d",table[i]);
}
}

    
