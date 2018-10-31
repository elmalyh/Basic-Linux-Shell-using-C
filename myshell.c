#include "myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define MAX_CHAR 100
#define MAX_WORD 10


int input_redirection_flag = 0;
int output_redirection_flag = 0;
int pipping_flag = 0;
char* input_file = NULL;
char* output_file = NULL;

int changeDir(char **args) {
	if(args[1]==NULL) {
		fprintf(stderr,"manque d'un argument\n");
	} else {
		printf("\n[Mini-Shell-->]$");
		if(chdir(args[1])!=0) {
			perror("erreur");
		}
	}
	return 1;
}

void clearScreen() {
	printf("\033[2J\033[1;1H");
	printf("\n[Mini-Shell-->]$");
}

void myhandler(int mysignal) {


printf("\nfailed attempt to close program , use ctrl+z instead\n");


}

int main() {
	char* args[MAX_WORD];
	char line[MAX_CHAR];
	char* pipping_args[MAX_WORD];
	int status;
	int pipefd[2];
	printf("\n[Mini-Shell-->]$");
	signal(SIGINT,myhandler);
	while (readandparseline(args,line,pipping_args)) {
		pipe(pipefd);
		pid_t pid=fork();
		if(pid==0) {
			if(input_redirection_flag==1 && input_file != NULL) {
				dup2(open(input_file,O_RDWR|O_CREAT,0777),0);
			}
			if(output_redirection_flag==1 && output_file != NULL) {
				dup2(open(output_file,O_RDWR|O_CREAT,0777),1);
			}
			if(pipping_flag == 1) {
				dup2(pipefd[1],1);
				close(pipefd[1]);
				close(pipefd[0]);
				execvp(args[0],args);
			}
			if(strcmp(args[0],"cd") ==0) {
				changeDir(args);
			}

			if(strcmp(args[0],"clear") ==0) {
				clearScreen();
			}

			execvp(args[0],args);

		} else {
			int pid;
			pid=fork();

			if(pid==0) {
					dup2(pipefd[0],0);
					close(pipefd[1]);
					close(pipefd[0]);
					execvp(pipping_args[0],pipping_args);
					//perror(args[0]);
		
			} else {
					close(pipefd[1]);
					close(pipefd[0]);
					wait(&pid);
					
			}
			wait(&pid);
			pid=wait(&status);
			input_redirection_flag = 0;
			output_redirection_flag = 0;	
			pipping_flag = 0;
			input_file = NULL;
			output_file = NULL;
			printf("\n[Mini-Shell-->]$");
		}
	}
	return 0;
}



int readandparseline(char* args[],char line[],char* pipping_args[]) {
	char* temp[MAX_WORD];
	int pos;
	int i=0;
	readline(line); 
	parseline(temp,line); 

	check_line(temp);

	pos=pipeandredirectionchecking(temp);
	
	while(i<pos) {
		args[i]=temp[i];
		i++;
	}
	args[i]=NULL;
	i++;
	if(pipping_flag==1) {
		int j=0;
		while(temp[i]!=NULL) {
			pipping_args[j]=temp[i];
			i++;
			j++;
		}
		pipping_args[j]=NULL;
	}
	
	return 1;
}



void readline(char line[]) {
	
	char* ret = fgets(line,MAX_CHAR,stdin);
	removeendofline(line);

	if(strcmp(line,"exit")==0 || ret == NULL) {
		exit(0);
	}

}

void removeendofline(char line[]) {

	int i=0;
	while(line[i] != '\n') {
		i++;
	}
	line[i]='\0';

}






int parseline(char* temp[],char line[]) {

int i=0;

temp[i]=strtok(line," ");

while(temp[i] != NULL) {
	i++;
	temp[i]=strtok(NULL," ");
}

return 1;

}



int pipeandredirectionchecking(char* temp[]) {

int i=0;

while(temp[i] != NULL) {
	if(strcmp(temp[i],">")==0) {
		output_redirection_flag=1;
		output_file=temp[i+1];
		return i;
	}
	if(strcmp(temp[i],"<")==0) {
		input_redirection_flag=1;
		input_file=temp[i+1];
		return i;
	}
	if(strcmp(temp[i],"|")==0) {
		pipping_flag=1;
		return i;
	}
	i++;
}
return i;
}


void check_line(char* temp[]) {
	int i=0;
	int pipe_cnt = 0;
	int output_redirection_cnt = 0;
	int input_redirection_cnt=0;
	int total_cnt = 0;

	if(temp[i]==NULL) {
		printf("NO COMMAND\n");
	}

	while(temp[i]!=NULL) {
		if(strcmp(temp[i],">")==0) {
			output_redirection_cnt++;
		}
		if(strcmp(temp[i],"<")==0) {
			input_redirection_cnt++;
		}
		if(strcmp(temp[i],"|")==0) {
			pipe_cnt++;
		}
		i++;
	}
	total_cnt=pipe_cnt+input_redirection_cnt+output_redirection_cnt;
	if(total_cnt>1) {
		printf("Err");
		temp[0]=NULL;
	}
}





















