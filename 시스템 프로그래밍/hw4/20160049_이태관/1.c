#include <stdio.h>
#include <pthread.h>

pthread_mutex_t odd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t even_mutex = PTHREAD_MUTEX_INITIALIZER;
int start = 1;
int MAX;
int num = 1;

int main(int argc, char *argv[]){
	
	if (argc != 2){
		fprintf(stderr,"arguments ERR\n");
		return -1;
	}

	MAX = atoi(argv[1]);

	pthread_t odd_thread;
	pthread_t even_thread;
	void *odd();
	void *even();
	if (MAX != 1)
	{
		pthread_create(&odd_thread,NULL,odd,NULL);
		pthread_create(&even_thread,NULL,even,NULL);
	}
	else{	
		printf("odd_thread : %d\n",num);
		return 0;
	}

	 pthread_join(odd_thread,NULL);
	 pthread_join(even_thread,NULL);
		 
	printf("complete all number\n");

	return 0;
}

void *odd(){
	while(num < MAX){
		pthread_mutex_lock(&odd_mutex);
		printf("odd_thread : %d\n",num);
		pthread_mutex_unlock(&even_mutex);
		num ++;
	}
	pthread_exit(NULL);
}

void *even(){
	sleep(1);
	while(num < MAX){
		pthread_mutex_lock(&even_mutex);
		printf("even thread : %d\n",num);
		pthread_mutex_unlock(&odd_mutex);
		num ++;
	}
	pthread_exit(NULL);
}
