#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<stdlib.h>


#define PTHREAD_NUM 16

unsigned long sum = 0;

void *thread(void *arg){
	for(int i = 0;i < 10000;i++)
		sum += 1;
	printf("after thread \n");
}

int main(void){
	printf("before ..sum is :%lu\n",sum);

	pthread_t pthread[PTHREAD_NUM];
	int ret;
	void *retval[PTHREAD_NUM];

	for(int i = 0;i < PTHREAD_NUM;i++){
		//printf("thread %d\n",pthread[i]);
		ret = pthread_create(&pthread[i],NULL,thread,NULL);
		if(ret != 0){
			perror("cause");
			printf("pthread %d crear fail\n",i+1);
		}
	}

	for(int i = 0;i < PTHREAD_NUM;i++){
		pthread_join(pthread[i],&retval[i]);
	}

	printf("after sum is: %lu\n",sum);
	return 0;

}

