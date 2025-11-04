#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int N;
int P;
int *v;
int ok_global = 1; // presupunem ca e palindrom

pthread_mutex_t mutex;
pthread_barrier_t barrier;

void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int start = thread_id * (N / (P * 2));
  int end = (thread_id + 1) * (N / (P * 2));
  if (thread_id == P - 1)
    end = N / 2;

  for (int i = start; i < end; i++) {
    if (v[i] != v[N - 1 - i]) {
      pthread_mutex_lock(&mutex);
      ok_global = 0;
      pthread_mutex_unlock(&mutex);
      break;
    }
  }

  pthread_barrier_wait(&barrier);

  if (thread_id == 0) {
    if (ok_global)
      printf("Vectorul ESTE palindrom.\n");
    else
      printf("Vectorul NU este palindrom.\n");
  }

  pthread_exit(NULL);
}

void init() {
  for (int i = 0; i < N; i++)
    v[i] = (i + 3) % 7;
}

void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Numar insuficient de parametri: ./program N P\n");
    exit(1);
  }

  N = atoi(argv[1]);
  P = atoi(argv[2]);
}

int main(int argc, char *argv[]) {
  get_args(argc, argv);

  v = malloc(sizeof(int) * N);
  if (!v) {
    perror("Eroare la malloc");
    exit(1);
  }

  init();

  pthread_t tid[P];
  int thread_id[P];

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, P);

  for (int i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (int i = 0; i < P; i++)
    pthread_join(tid[i], NULL);

  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);
  free(v);

  return 0;
}
