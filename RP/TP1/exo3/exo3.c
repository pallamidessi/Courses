
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<netdb.h>
#include<errno.h>

typedef int bool;


#define TRUE 1
#define FALSE 0

int main(int argc, char **argv){

	int sockfd,sockfd3;
	struct sockaddr* server_web;
	struct sockaddr_in browser;
	socklen_t addrlen,browser_addrlen;
	struct addrinfo hints;
	struct addrinfo* result;
	bool success=FALSE;
	char buffer[1024*30];

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

	if((sockfd3 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	
	// creation of the browser  adress in struct sockaddr
	browser.sin_family = AF_INET;
	browser.sin_port   = htons(80);
	browser.sin_addr.s_addr =INADDR_ANY;
	browser_addrlen		 = sizeof(struct sockaddr_in);
	

	//retriving of the ipv4 adress of the parameter given to the program 
	hints.ai_family=AF_INET;				//ipv4
	hints.ai_socktype=SOCK_STREAM;	//tcp 
	hints.ai_flags=0;			
	hints.ai_protocol=0;
	hints.ai_canonname=NULL;
	hints.ai_addr=NULL;
	hints.ai_next=NULL;

	if(getaddrinfo(argv[1],"80",&hints,&result)!=0){
		perror("getaddrinfo");
		close(sockfd);
		exit(EXIT_FAILURE);
	}


	//trying to connect to the host

	while(result!=NULL){
		if(connect(sockfd,result->ai_addr,result->ai_addrlen) != -1){
			server_web=result->ai_addr;
			addrlen=result->ai_addrlen;
			result=result->ai_next;
			success=TRUE;
			break;
		}
	}

	if(success==FALSE){
		printf("Error \n");
		close(sockfd);
		exit(1);
	}

	//prompt the user about what page he want on the connected web server

	printf("page : \n");
	strcpy(buffer,"GET ");
	scanf("%s",buffer+4);

	strcat(buffer," HTTP/1.1 \r\n");
	printf("\n %s \n",buffer);

	//send the GET request to the web server with the correct formed URL
	send(sockfd,buffer,strlen(buffer),0);

	//wait to receive the requested page
	if(recv(sockfd,buffer,1024*30,0)==0){
		printf("L'host a ferme son socket \n");
	}

	printf("%s",buffer);
	close(sockfd);
	return 0;	
}
