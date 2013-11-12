LINUX= -lnsl
CC=gcc
CFLAGS=-Wall -g 
#----------------------------------------------
all : serveur client

include: include.o xdr_entiers.o include.h
	gcc -o include include.o xdr_entier.o ${LINUX}
#----------------------------------------------
serveur : serveur.o xdr_entiers2.o include.o
	gcc -o serveur serveur.o xdr_entiers2.o include.o ${LINUX}
#----------------------------------------------
client : client.o xdr_entiers2.o include.o
	gcc -o client client.o xdr_entiers2.o include.o ${LINUX}
#----------------------------------------------
clean :
	/bin/rm -f *.o serveur client *~ core* pok* *.ps
