#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int N, P;
int *v;

long int *suma;
long long int *prod;
int *minv;
int *maxv;

pthread_barrier_t barrier;

void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Numar insuficient de parametri: ./program N P v1 v2 ...\n");
    exit(1);
  }
  N = atoi(argv[1]);
  P = atoi(argv[2]);
  v = malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++)
    v[i] = atoi(argv[i + 3]);
}

void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int start = thread_id * (double)(N / P);
  int end = (thread_id + 1) * (double)(N / P);
  if (thread_id == P - 1)
    end = N;

  long int sum_loc = 0;
  long long int prod_loc = 1;
  int min_loc = v[start];
  int max_loc = v[start];

  for (int i = start; i < end; i++) {
    sum_loc += v[i];
    prod_loc *= v[i];
    if (v[i] < min_loc)
      min_loc = v[i];
    if (v[i] > max_loc)
      max_loc = v[i];
  }

  suma[thread_id] = sum_loc;
  prod[thread_id] = prod_loc;
  minv[thread_id] = min_loc;
  maxv[thread_id] = max_loc;

  // așteaptă ca toate threadurile să termine calculele
  pthread_barrier_wait(&barrier);

  if (thread_id == 0) {
    long int total_sum = 0;
    long long int total_prod = 1;
    int global_min = minv[0];
    int global_max = maxv[0];
    for (int i = 0; i < P; i++) {
      total_sum += suma[i];
      total_prod *= prod[i];
      if (minv[i] < global_min)
        global_min = minv[i];
      if (maxv[i] > global_max)
        global_max = maxv[i];
    }
    double avg = (double)total_sum / N;

    if (P >= 1)
      printf("sum: %ld\n", total_sum);
    if (P >= 2)
      printf("prod: %lld\n", total_prod);
    if (P >= 3)
      printf("min: %d\n", global_min);
    if (P >= 4)
      printf("max: %d\n", global_max);
    if (P >= 5)
      printf("avg: %.0f\n", avg);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  get_args(argc, argv);

  pthread_t tid[P];
  int thread_id[P];

  suma = malloc(sizeof(long int) * P);
  prod = malloc(sizeof(long long int) * P);
  minv = malloc(sizeof(int) * P);
  maxv = malloc(sizeof(int) * P);

  pthread_barrier_init(&barrier, NULL, P);

  for (int i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (int i = 0; i < P; i++)
    pthread_join(tid[i], NULL);

  pthread_barrier_destroy(&barrier);

  free(v);
  free(suma);
  free(prod);
  free(minv);
  free(maxv);

  return 0;
}
