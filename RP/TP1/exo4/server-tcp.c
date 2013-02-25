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
#include<select.h>
#include<fcntl.h>

#define MAX_CLIENT 16

int main(int argc, char **argv)
{
	int i,j;
	int sockfd, sockfd2;
	int max=0;
	int nbr_client=0;
	fd_set rdclient; 
	int list_client[MAX_CLIENT];
	socklen_t addrlen;
	char buf[1024];

	FD_ZERO(&rdclient);

	struct sockaddr_in my_addr;

	// check the number of args on command line
	if(argc != 2)
	{
		printf("USAGE: %s port_num\n", argv[0]);
		exit(-1);
	}

	// socket factory
	if((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// init local addr structure and other params
	my_addr.sin_family      = AF_INET;
	my_addr.sin_port        = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addrlen                 = sizeof(struct sockaddr_in);
	memset(buf,'\0',1024);

	// bind addr structure with socket
	if(bind(sockfd,(struct sockaddr*)&my_addr,addrlen) == -1)
	{
		perror("bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	//Set sockfd to be non-blocking
	fcntl(sockfd,F_SETFL,O_NONBLOCK);

	// set the socket in passive mode (only used for accept())
	// and set the list size for pending connection
	listen(sockfd,SOMAXCONN);
	
	if(sockfd2>max){
		max=sockfd2;
	}

	FD_SET(sockfd);

	while(1){
		if(nbr_client<MAX_CLIENT){

			if((select(max+1,rdclient,NULL,NULL,NULL))>1)
				sockfd2 = accept(sockfd,(struct sockaddr*)&my_addr,&addrlen);

			if(sockfd2>max){
				max=sockfd2;
			}

			FD_SET(sockfd2,&rdclient);

			//Adding the newly connected client to the list of client
			list_client[nbr_client]=sockfd2;
			nbr_client++;

			for(i=0;i<nbr_client;i++){
				if(FD_ISSET(list_client[i])){
					recv(list_client[i],buf,1024,0);
					FD_SET(list_client[i],&rdclient);

					for(j=0;j<nbr_client;j++){
						if(j!=i)
							send(list_client[i],buf,1024,0);	
					}
				}
			}
		}

		// fermeture des sockets
		close(sockfd);
		close(sockfd2);

		return 0;
	}
