#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define STR_SIZE 512
#define true 1
#define false 0

typedef struct proc {
	unsigned int pid;
	unsigned int ppid;
	char cmd[STR_SIZE];
	unsigned int uid;
	unsigned int gid;
	char state[STR_SIZE];
	unsigned int v_memory_size;
}process_t;

int strcmp_debut(char* s,char* name_champ){
	int size=strlen(name_champ);
	int i;

	for(i=0;i<size;i++){
		if(s[i]!=name_champ[i])
			return false;
	} 
	return true;
}

process_t info_pid(char* pid){
	process_t info_process;
	char buffer[256];
	char buffer_traitement[256];
	int i=0;
	FILE* status;
	char path[256]="/proc/";

	strcat(path,pid);
	strcat(path,"/status");

	if((status=fopen(path,"r"))==NULL){
		perror("Open\n");
		exit(1);
	}

	while(fgets(buffer,256,status)!=NULL){ 
		int size_line=strlen(buffer);
		
		if(strcmp_debut(buffer,"Pid")){
			strncpy(buffer_traitement,buffer+5,size_line-6);
			buffer_traitement[size_line-6]='\0';
			info_process.pid=atoi(buffer_traitement);
		}
		else if(strcmp_debut(buffer,"Ppid")){
			strncpy(buffer_traitement,buffer+5,size_line-6);
			buffer_traitement[size_line-6]='\0';
			info_process.ppid=atoi(buffer_traitement);
		}
		else if(strcmp_debut(buffer,"Uid")){
			strncpy(buffer_traitement,buffer+5,size_line-6);
			buffer_traitement[size_line-6]='\0';
			info_process.uid=atoi(buffer_traitement);
		}
		else if(strcmp_debut(buffer,"Gid")){
			strncpy(buffer_traitement,buffer+5,size_line-6);
			buffer_traitement[size_line-6]='\0';
			info_process.gid=atoi(buffer_traitement);
		}
		else if(strcmp_debut(buffer,"State")){
			strncpy(buffer_traitement,buffer+6,size_line-7);
			buffer_traitement[size_line-7]='\0';
			strcpy(info_process.state,buffer_traitement);
		}
		else if(strcmp_debut(buffer,"VmSize")){
			strncpy(buffer_traitement,buffer+11,size_line-13);
			buffer_traitement[size_line-13]='\0';
			
			while(buffer_traitement[i]!=' ')
				i++;

			buffer_traitement[i]='\0';
			info_process.v_memory_size=atoi(buffer_traitement);
		}
		else if(strcmp_debut(buffer,"Name")){
			strncpy(buffer_traitement,buffer+6,size_line-8);
			buffer_traitement[size_line-8]='\0';
			strcpy(info_process.cmd,buffer_traitement);
		}
	}

	return info_process;
}

void print_process_t(process_t p){
	
	printf("Pid:\t %d\n",p.pid);
	printf("Ppid:\t %d\n",p.ppid);
	printf("Gid:\t %d\n",p.gid);
	printf("Uid:\t %d\n",p.uid);
	printf("State: %s\n",p.state);
	printf("VmSize: %d\n",p.v_memory_size);
	printf("Cmd:\t %s\n",p.cmd);
}

int main (int argc,char* argv[]){
	
	if(argc!=2){
		printf("Usage: %s pid",argv[0]);
		exit(1);
	}

	process_t pid=info_pid(argv[1]);
	
	print_process_t(pid);
	
	return 0;	
}
