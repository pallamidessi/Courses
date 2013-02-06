
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

typedef bool int;
#define TRUE 1
#define FALSE 0
int main(int argc, char **argv){
	
	int sockfd;
	struct sockaddr_in server_web;
	socklen_t addrlen;
	struct addrinfo hints;
	struct addrinfo* result;
	bool success=FALSE;
	char buffer[1024];

	//check the number of args given to the program 
	if(argc != 2){
		printf("USAGE: %s @server \n", argv[0]);
		exit(-1);
   }
	
	//socket creation

	if((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	// creation of the remote web server adress in struct sockaddr
	server.sin_family = AF_INET;
	server.sin_port   = htons(80);
	addrlen           = sizeof(struct sockaddr_in);
	
	//retriving of the ipv4 adress of the parameter given to the program 
	hints.ai_family=AF_INET;				//ipv4
	hints.ai_socktype=SOCK_STREAM;	//tcp 
	hints.ai_flags=AI_PASSIVE;			
	hints.ai_protocol=0;
	hints.ai_canonname=NULL;
	hints.ai_addr=NULL;
	hints.ai_next=NULL;
	
	if(getaddrinfo(argv[1],"80",&hints,&res)!=0){
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}


	//trying to connect to the host
	while(result->ai_next!=NULL){
		if(connect(sockfd,result->ai_addr,result->ai_addrlen) != -1)
			success=TRUE;
			break;
	}
	
	if(success==FALSE)
		printf("Error \n");
		exit(1);

	//prompt the user about what page he want on the connected web server

	printf("page : \n");
	scanf("%s",buffer);
	
	//send the GET request to the web server with the correct formed URL
	send(sockfd,buffer,strlen(buffer));
	
	//wait to receive the requested page
	recv(sockfd,buffer,1024);


	return 0;	
}
