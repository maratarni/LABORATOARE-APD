#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int N, P;
int *v;

long long sum_result = 0;
long long prod_result = 1;
int min_result;
int max_result;
double avg_result;

pthread_barrier_t barrier;

void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: ./testapd N P v1 v2 ...\n");
    exit(1);
  }
  N = atoi(argv[1]);
  P = atoi(argv[2]);
  v = malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++)
    v[i] = atoi(argv[i + 3]);
}

void *thread_function(void *arg) {
  int id = *(int *)arg;
  long long sum_loc = 0, prod_loc = 1;
  int min_loc = v[0], max_loc = v[0];

  // fiecare thread are o operație diferită
  if (id == 0) { // SUM
    for (int i = 0; i < N; i++)
      sum_loc += v[i];
    sum_result = sum_loc;
  }

  else if (id == 1 && P >= 2) { // PRODUCT
    for (int i = 0; i < N; i++)
      prod_loc *= v[i];
    prod_result = prod_loc;
  }

  else if (id == 2 && P >= 3) { // MIN
    for (int i = 0; i < N; i++)
      if (v[i] < min_loc)
        min_loc = v[i];
    min_result = min_loc;
  }

  else if (id == 3 && P >= 4) { // MAX
    for (int i = 0; i < N; i++)
      if (v[i] > max_loc)
        max_loc = v[i];
    max_result = max_loc;
  }

  else if (id == 4 && P >= 5) { // AVG
    for (int i = 0; i < N; i++)
      sum_loc += v[i];
    avg_result = (double)sum_loc / N;
  }

  // sincronizare: toate threadurile trebuie să termine calculele
  pthread_barrier_wait(&barrier);

  // doar threadul 0 afișează rezultatele
  if (id == 0) {
    if (P >= 1)
      printf("sum: %lld\n", sum_result);
    if (P >= 2)
      printf("prod: %lld\n", prod_result);
    if (P >= 3)
      printf("min: %d\n", min_result);
    if (P >= 4)
      printf("max: %d\n", max_result);
    if (P >= 5)
      printf("avg: %.0f\n", avg_result);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  get_args(argc, argv);

  pthread_t threads[P];
  int ids[P];
  pthread_barrier_init(&barrier, NULL, P);

  for (int i = 0; i < P; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, thread_function, &ids[i]);
  }

  for (int i = 0; i < P; i++)
    pthread_join(threads[i], NULL);

  pthread_barrier_destroy(&barrier);
  free(v);
  return 0;
}
