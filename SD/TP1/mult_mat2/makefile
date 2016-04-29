LINUX= -lnsl
CC=gcc
CFLAGS=-Wall -g
#----------------------------------------------
all : serveur client 
#----------------------------------------------
serveur : serveur.o xdr_entiers2.o
	gcc -o serveur serveur.o xdr_entiers2.o ${LINUX}
#----------------------------------------------
client : client.o xdr_entiers2.o
	gcc -o client client.o xdr_entiers2.o ${LINUX}
#----------------------------------------------
clean :
	/bin/rm -f *.o serveur client *~ core* pok* *.ps
