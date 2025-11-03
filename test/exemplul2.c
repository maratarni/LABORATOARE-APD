
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *array;
int array_size;
pthread_barrier_t barrier;

void *thread_function(void *arg) {
  int thread_id = *(int *)arg;

  if (thread_id == 0) {
    // MIN
    int min = array[0];
    for (int i = 1; i < array_size; i++) {
      if (array[i] < min)
        min = array[i];
    }
    printf("minim = %d\n", min);
  }
  // pthread_barrier_wait(&barrier);
  if (thread_id == 1) {
    // MAX
    int max = array[0];
    for (int i = 1; i < array_size; i++) {
      if (array[i] > max)
        max = array[i];
    }

    printf("maxim = %d\n", max);
  }
  // pthread_barrier_wait(&barrier);
  if (thread_id == 2) {
    // SUMA
    long long suma = 0;
    for (int i = 0; i < array_size; i++) {
      suma += array[i];
    }

    printf("suma = %lld\n", suma);

  } // pthread_barrier_wait(&barrier);
  if (thread_id == 3) {
    // PRODUS
    long long produs = 1;
    for (int i = 0; i < array_size; i++) {
      produs = (produs * array[i]) % 1337;
    }
    printf("produs = %lld\n", produs);
  }
  // pthread_barrier_wait(&barrier);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <array_size>\n", argv[0]);
    exit(1);
  }

  array_size = atoi(argv[1]);
  array = malloc(array_size * sizeof(int));

  srand(42);
  for (int i = 0; i < array_size; i++) {
    array[i] = rand() % 100;
  }

  // pthread_barrier_init(&barrier, NULL, 4);

  pthread_t threads[4];
  int ids[4] = {0, 1, 2, 3};

  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], NULL, thread_function, &ids[i]);
  }

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }

  // pthread_barrier_destroy(&barrier);
  free(array);

  return 0;
}