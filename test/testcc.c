#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int N;
int P;
int *v;

pthread_mutex_t mutex;
pthread_barrier_t barrier;
// ne formam vectorul
void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int i, j;
  int start = 2 + thread_id * (double)(P + 1 - 2 + 1) / P;
  int end = 2 + (thread_id + 1) * (double)(P + 1 - 2 + 1) / P - 1;

  if (thread_id == P - 1)
    end = P + 1;
  for (j = 2; j <= P + 1; j++) {
    pthread_barrier_wait(&barrier); // toți sincronizați pe același j

    if (j >= start && j <= end) {
      pthread_mutex_lock(&mutex);

      printf("%d: ", j);
      for (i = 0; i < N; i++) {
        if (v[i] % j == 0)
          printf("%d ", v[i]);
      }
      printf("\n");

      pthread_mutex_unlock(&mutex);
    }

    pthread_barrier_wait(
        &barrier); // toți așteaptă să se termine printul curent
  }
  pthread_exit(NULL);
}

void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Numar insuficient de parametri: ./program N P n1...nN\n");
    exit(1);
  }

  N = atoi(argv[1]);
  P = atoi(argv[2]);

  v = (int *)malloc(sizeof(int) * N);
  int i;
  for (i = 0; i < N; i++) {
    v[i] = atoi(argv[i + 3]);
  }
}

int main(int argc, char *argv[]) {
  int i;
  get_args(argc, argv);
  pthread_t tid[P];
  int thread_id[P];
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, P);
  for (i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }
  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }
  return 0;
}