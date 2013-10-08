LINUX=-lnsl
#----------------------------------------------
all : serveur client
#----------------------------------------------
serveur : serveur.o
	gcc -o serveur serveur.o ${LINUX}
serveur.o : serveur.c include.h
	gcc -Wall -c serveur.c
#----------------------------------------------
client : client.o
	gcc -o client client.o -lrpcsvc ${LINUX}
client.o : client.c include.h
	gcc -Wall -c client.c
#----------------------------------------------
clean :
	/bin/rm -f *.o serveur client *~ pok* *.ps
