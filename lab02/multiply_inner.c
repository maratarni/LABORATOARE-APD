#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int N;
int P;
int **a;
int **b;
int **c;
pthread_barrier_t bariera;
pthread_mutex_t mutex;
// TODO: paralelizati operatia din comentariul din functie
// in interiorul functiei respective
void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int start = thread_id * (N / P);
  int end = ((thread_id + 1) * N / P < N) ? ((thread_id + 1) * N / P) : N;

  int sum_partiala;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      sum_partiala = 0;
      for (int k = start; k < end; k++) {
        sum_partiala = sum_partiala + a[i][k] * b[k][j];
      }
      pthread_mutex_lock(&mutex);
      c[i][j] += sum_partiala;
      pthread_mutex_unlock(&mutex);
    }
  }

  pthread_exit(NULL);
}

void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Numar insuficient de parametri: ./program N P\n");
    exit(1);
  }

  N = atoi(argv[1]);
  P = atoi(argv[2]);
}

void init() {
  a = malloc(sizeof(int *) * N);
  b = malloc(sizeof(int *) * N);
  c = malloc(sizeof(int *) * N);

  if (a == NULL || b == NULL || c == NULL) {
    printf("Eroare la malloc!");
    exit(1);
  }

  int i, j;
  for (i = 0; i < N; i++) {
    a[i] = malloc(sizeof(int) * N);
    b[i] = malloc(sizeof(int) * N);
    c[i] = malloc(sizeof(int) * N);

    if (a[i] == NULL || b[i] == NULL || c[i] == NULL) {
      printf("Eroare la malloc!");
      exit(1);
    }
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      c[i][j] = 0;

      if (i <= j) {
        a[i][j] = 1;
        b[i][j] = 1;
      } else {
        a[i][j] = 0;
        b[i][j] = 0;
      }
    }
  }
}

void print(int **mat) {
  int i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      printf("%i\t", mat[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int i;
  struct timeval start, end;
  double execution;
  get_args(argc, argv);
  init();

  pthread_t tid[P];
  int thread_id[P];
  pthread_barrier_init(&bariera, NULL, P);
  pthread_mutex_init(&mutex, NULL);
  gettimeofday(&start, NULL);
  for (i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }
  gettimeofday(&end, NULL);

  execution = (end.tv_sec - start.tv_sec) * 1000.0 +
              (end.tv_usec - start.tv_usec) / 1000.0;
  printf("Execution time: %f ms\n", execution);

  print(c);
  pthread_barrier_destroy(&bariera);
  pthread_mutex_destroy(&mutex);
  return 0;
}
