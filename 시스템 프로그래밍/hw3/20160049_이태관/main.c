#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
char *env1[2] = {"USER=Lee Tae Gwan",NULL}; 

int main(int argc, char **argv){
	char buf[100];
	char nowpath[100];
	char *vec[] = {"show_env",0};
	int status;
	strcpy(buf, getcwd(nowpath,80));
	strcat(buf, "/show_env");
	
	int pid = fork();
	if (strcmp(argv[1],"1") == 0){
		if (pid == 0)
		execlp("./show_env","show_env",NULL);
	}
	else if (strcmp (argv[1],"2") == 0){
		if (pid == 0)
		execle(buf,"show_env",NULL,env1);
	}
	else if (strcmp (argv[1],"3") == 0){
		if (pid == 0)
		execvp("./show_env",vec);
	}
	else if (strcmp (argv[1],"4") == 0){
		if (pid == 0)
		execve(buf,vec,env1);
	}
	wait(&status);
	printf("Print USER of env_var Complete!!\n");
	return 0;


}
