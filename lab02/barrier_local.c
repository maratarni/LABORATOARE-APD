#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
typedef struct{
	pthread_barrier_t *bariera;
    int id;
}ThreadArgs;

//pthread_barrier_t bariera;
// TODO: de folosit bariera in aceasta functie
void *f(void *arg)
{
	ThreadArgs *data = (ThreadArgs*)arg;
    int thread_id = data->id;
    pthread_barrier_t *bariera = data->bariera;

	if (thread_id == 1) {
		printf("1\n");
	}

	pthread_barrier_wait(bariera);

	if (thread_id == 0) {
		printf("2\n");
	}
	//pthread_barrier_wait(&bariera);

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	ThreadArgs arguments[NUM_THREADS];
    pthread_barrier_t bariera;
	pthread_barrier_init(&bariera, NULL, NUM_THREADS);

	for (i = 0; i < NUM_THREADS; i++) {
		arguments[i].id = i;
        arguments[i].bariera = &bariera;
		r = pthread_create(&threads[i], NULL, f, &arguments[i]);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}
	pthread_barrier_destroy(&bariera);
	return 0;
}