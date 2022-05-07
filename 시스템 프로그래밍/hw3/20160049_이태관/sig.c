#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
void sigUSR(int signo);

char buf[50];

int main(int argc, char **argv){
	strcpy(buf,argv[1]);
	printf("Start : [%s] - %d\n",argv[1],getpid());
	signal(SIGUSR1,(void *)sigUSR);
	signal(SIGUSR2,(void *)sigUSR);
	while(1){
		pause();
	}
	
}

void sigUSR(int signo){
	if (signo == SIGUSR1){
		if (strcmp(buf,"sigint") == 0 ){
			printf("Running [sigint] - %d\n",getpid());
		}
		else{
			printf("Running [sigtstp] - %d\n",getpid());
		}

	}
	else if (signo == SIGUSR2){
		printf("Exit [%s] - %d\n",buf,getpid());
		raise(SIGKILL);
	}

}




