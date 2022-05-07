#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
void sigINT (int signo);
void sigTSTP (int signo);
void sigUSR (int signo);

char pidlist[100][100];
int indx = 0;

int main(void){
	signal(SIGINT,(void *) sigINT);
	signal(SIGTSTP,(void *) sigTSTP);
	signal(SIGUSR1,(void *) sigUSR);
	signal(SIGUSR2,(void *) sigUSR);

	while(1);

	return 0;
	
}

void sigINT(int signo){
	int i=0;
	char buf[100];
	char nowpath[100];
	char list[100];
	strcpy(buf, getcwd(nowpath,100));
	strcat(buf, "/sig");
	int pid = fork();
	if (pid == 0){
		
		execl(buf,"sig","sigint", (char*) 0);
	}
	else if (pid > 0){
		sprintf(list, "%d",pid);
		strcpy(pidlist[indx],list);
		indx ++;
	}
}

void sigTSTP(int signo){	
	char buf[100];
	char nowpath[100];
	char list[30];
	strcpy(buf, getcwd(nowpath,100));
	strcat(buf, "/sig");
	int pid = vfork();
	int i;

	if (pid == 0){
		execl(buf,"sig","sigtstp", (char*) 0);
	}
	else if (pid >0){
		sprintf(list,"%d",pid);
		strcpy(pidlist[indx], list);
		indx ++;
	}
}

void sigUSR (int signo){
	int i;
	if (signo == SIGUSR1){
		for (i = 0 ; i < indx ; i++){
			kill(atoi(pidlist[i]),SIGUSR1);
		}
	}
	else if (signo == SIGUSR2){
		for (i=0 ; i< indx ; i++){
			kill(atoi(pidlist[i]),SIGUSR2);
		}
	}
}

