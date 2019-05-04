#include <stdio.h>
#include <pthread.h>

void* start_routine(void* arg) {
	pthread_mutex_t* mutex = (pthread_mutex_t*) arg;
	pthread_mutex_lock(mutex);
	printf("\n\nIn thread\n\n");
	pthread_mutex_unlock(mutex);
	return NULL;
}

int main() {
	printf("Experimenting\n");
	pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;


	pthread_t myThread;
	pthread_create(&myThread, NULL, start_routine, (void*) &myMutex);

	int i;
	for (i=0;i<2000;i++){
		printf("x");
	}
	pthread_mutex_lock(&myMutex);
	for(i=0;i<1000;i++){
		printf("y");
	}
	printf("\n");
	pthread_mutex_unlock(&myMutex);

	pthread_join(myThread, NULL);
	return 0;
}
