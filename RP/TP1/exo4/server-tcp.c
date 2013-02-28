/**
 * @file server-tcp.c
 * @author Julien Montavont
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Simple program that creates an IPv4 TCP socket waits for the
 * connection of a client and the reception of a string from that
 * client. The program takes a single parameter which is the local
 * communication port. The IPv4 addr associated to the socket will be
 * all available addr on the host (use INADDR_ANY maccro).
 */


#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/select.h>
#include<fcntl.h>
#include<signal.h>

#define MAX_CLIENT 16
#define TRUE 1
#define FALSE 0

typedef int bool;
int list_client[MAX_CLIENT];
int sockfd;

/*Example of command the chat can support */
void who_is_connected(int sockfd,int* list_client,char** nom_client,int max_client){
	int i=0;

	for(i=0;i<MAX_CLIENT;i++){
		if(list_client[i]!=0 && sockfd!=list_client[i]){
			send(sockfd,nom_client[i],16,0);
			
		}
	}
}

/* Close the server and all opened sockets when a SIGINT is received and send a terminaison (interpreted) 
 * string to the connected clients*/

void fin(int sig){
	int i;

	close(sockfd);	
	
	for(i=0;i<MAX_CLIENT;i++){
		if(list_client[i]!=0){
			send(list_client[i],"000/END",8,0);
			close(list_client[i]);
		}
	}

	exit(0);
}


int main(int argc, char **argv)
{
	int i,j;
	int sockfd2;
	int max=0;
	int nbr_client=0;
	bool next_is_name=FALSE,client_disconnected=FALSE;
	fd_set rdclient; 
	char name_client[MAX_CLIENT][16];
	socklen_t addrlen;
	char buf[1024];
	char buf2[1024];


	struct sockaddr_in my_addr;
	struct sigaction terminaison;

	/*Initializing the client array */
	for(i=0;i<MAX_CLIENT;i++){
		list_client[i]=0;
	}

	/* check the number of args on command line*/
	if(argc != 2)
	{
		printf("USAGE: %s port_num\n", argv[0]);
		exit(-1);
	}

	/* socket factory*/
	if((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	/*Signal handler in case of ^C, to close the sockets*/
	terminaison.sa_handler=fin;
	sigfillset(&terminaison.sa_mask);
	terminaison.sa_flags=0;
		
	sigaction(SIGINT,&terminaison,NULL);

	/* init local addr structure and other params */
	my_addr.sin_family      = AF_INET;
	my_addr.sin_port        = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addrlen                 = sizeof(struct sockaddr_in);
	memset(buf,'\0',1024);

	/* bind addr structure with socket */
	if(bind(sockfd,(struct sockaddr*)&my_addr,addrlen) == -1)
	{
		perror("bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/*Set sockfd to be non-blocking*/
	fcntl(sockfd,F_SETFL,O_NONBLOCK);

	/* set the socket in passive mode (only used for accept())
	 * and set the list size for pending connection*/
	listen(sockfd,SOMAXCONN);

	max=sockfd;


	while(1){

		FD_ZERO(&rdclient);
		FD_SET(sockfd,&rdclient);

		for(i=0;i<nbr_client;i++){
			if(list_client[i]!=0){
				FD_SET(list_client[i],&rdclient);
			}
		}

		if(nbr_client<MAX_CLIENT){
			if((select(max+1,&rdclient,NULL,NULL,NULL))>=1){
				if(FD_ISSET(sockfd,&rdclient)){
					sockfd2 = accept(sockfd,(struct sockaddr*)&my_addr,&addrlen);

					if(sockfd2>max){
						max=sockfd2;
					}
					printf("nouveaux client\n");

					/*Adding the newly connected client to the list of client*/
					
					i=0;
					
					while(list_client[i]!=0 && i<MAX_CLIENT){
						i++;
					}

					list_client[i]=sockfd2;

					nbr_client++;
					next_is_name=TRUE;
				}

				/* Check whose fd changed and received from him and then send the message to every
				 * other client */

				else{
					for(i=0;i<nbr_client;i++){
						if(list_client[i]!=0){
							if(FD_ISSET(list_client[i],&rdclient)){
								if(recv(list_client[i],buf,1024,0)==0){
									client_disconnected=TRUE;
								}
								else if(strcmp(buf,"/who")==0){
									who_is_connected(list_client[i],list_client,(char **) name_client,MAX_CLIENT);
									break;
								}	
								else
									printf("%s\n",buf);

								/* Notify everyone if a new user connect */
								if(next_is_name==TRUE){
									strcpy(name_client[i],buf);
									next_is_name=FALSE;
									strcat(buf," vient de rejoindre le chat !");
									strcpy(buf2,buf);
								}
								/* Notify everyone if an user disconnect */
								else if(client_disconnected==TRUE){
									list_client[i]=0;
									nbr_client--;

									strcpy(buf2,name_client[i]);
									strcat(buf2," vient de se deconnecter !");
									client_disconnected=FALSE;
								}


								/* Send the message to everyone with the sender name before */
								else {
									strcpy(buf2,name_client[i]);
									printf("%s\n",name_client[i]);
									strcat(buf2,": ");
									strcat(buf2,buf);
								}
								/* Send loop*/
								for(j=0;j<MAX_CLIENT;j++){
									if(list_client[j]!=0 && j!=i){
										if(send(list_client[j],buf2,1024,0)==-1){
											perror("send : ");
										}	
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
