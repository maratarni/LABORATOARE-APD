#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int N;
int P;
int *v;

void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Numar insuficient de parametri: ./program N P n1 n2...\n");
    exit(1);
  }
  N = atoi(argv[1]);
  P = atoi(argv[2]);
  v = (int *)malloc(sizeof(int) * N);
  int i;
  for (i = 0; i < N; i++)
    v[i] = atoi(argv[i + 3]);
}
void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int i, j;

  int start = 2 + thread_id * (P + 1 - 2 + 1);
  int end = 2 + thread_id * (P + 1 - 2 + 1) - 1;
  if (thread_id == P - 1)
    end = P + 1;
  for (i = 2; i < P + 1; i++) {
    printf("%d: ", i);
    for (j = 0; j < N; j++) {
      if (v[j] % i == 0)
        printf("%d", v[j]);
    }
  }
}
int main(int argc, char *argv[]) {
  int i;
  get_args(argc, argv);
  pthread_t tid[P];
  int thread_id[P];

  for (i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }
  return 0;
}