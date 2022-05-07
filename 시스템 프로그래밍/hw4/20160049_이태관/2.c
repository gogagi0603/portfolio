/*

	������ �����尡 �ѹ��� ���� �� ���Ŀ� cursor�� ���� �������� �ö� �� �� �����尡 ���� �Ǳ� ������ arr[cursor]��ŭ ��ٷȴٰ� ǥ���ϴ°� �ð��� �����ؼ� �ȵƽ��ϴ�.
	�̰Ÿ� �ذ��ϸ� �� �� ������ �ڲ� argv[1] ���ķ� ����� ����� �ǳ׿� 

*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int cursor = 0;
int read_cur = 0;
int *arr;
int MAX;
int thread_count;

pthread_cond_t ready[1024]; 
pthread_mutex_t mutex[1024];
pthread_mutex_t cur = PTHREAD_MUTEX_INITIALIZER;
int main(int argc, char *argv[]){
	int i;
	srand(time(NULL));
	int *mallocarr = (int*)malloc(sizeof(int) * atoi(argv[2]));
	arr = mallocarr;
	char buf[256];
	thread_count = atoi(argv[1]);
	MAX = atoi(argv[2]);
	void *thread_handle();
	pthread_t pthread[thread_count];

	pthread_cond_t condition[thread_count];

	for (i = 0 ; i < MAX ; i++)
	{
		arr[i] = (rand()%6) + 5 ;
	}

	for (i = 0 ; i < MAX ; i++)
	{
		printf("%d ", arr[i]);
		
	}

	
	printf("\n");
	cursor = 0;
	for (i = 0 ; i< thread_count ; i++){
		pthread_create(&pthread[i],NULL,thread_handle, (void *) i);
	}

	for (i = 0 ; i< thread_count ; i++){
		pthread_mutex_init(&mutex[i],NULL);
		pthread_cond_init(&ready[i],NULL);
	}

	for (i = 0 ; i< thread_count; i++){
		pthread_join(pthread[i],NULL);
	}


	return 0;
}

void *thread_handle(void* thread_num){
	int index = (int *) thread_num;

	while (1){
		struct timeval now;
		struct timespec ts;
		
		pthread_mutex_trylock(&cur);
		 read_cur = cursor;
		gettimeofday (&now,NULL);
		ts.tv_sec = now.tv_sec + arr[cursor];
		ts.tv_nsec = now.tv_usec * 1000;
		 cursor += 1;
		pthread_mutex_unlock(&cur);
		pthread_cond_timedwait(&ready[index],&mutex[index], &ts);

		printf("%x - element[%d] : %d\n",(unsigned int)pthread_self(),read_cur,arr[read_cur]);
	
		if (MAX <= cursor){
			break;
		}
	}
	pthread_exit(NULL);
}

