#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int N;
int P;
int X;
int *array;
int *Nr_aparitii;
int *Suma_pozitii;

pthread_barrier_t barrier;

void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int i;
  int start = thread_id * (double)(N / P);
  int end = (thread_id + 1) * (double)(N / P);
  if (thread_id == P - 1)
    end = N;

  int nr_aparitii_thread = 0;
  int suma_pozitii_thread = 0;

  for (i = start; i < end; i++) {
    if (array[i] == X) {
      nr_aparitii_thread++;
      suma_pozitii_thread += i;
    }
  }

  Nr_aparitii[thread_id] = nr_aparitii_thread;
  Suma_pozitii[thread_id] = suma_pozitii_thread;

  pthread_barrier_wait(&barrier);

  if (thread_id == 0) {
    int suma = 0, aparitii = 0;
    for (i = 0; i < P; i++) {
      suma += Suma_pozitii[i];
      aparitii += Nr_aparitii[i];
    }
    printf("%d %d\n", aparitii, suma);
  }

  pthread_exit(NULL);
}

void get_args(int argc, char **argv) {
  if (argc < 4) {
    printf("Numar insuficient de parametri: ./testapd N P X\n");
    exit(1);
  }

  N = atoi(argv[1]);
  P = atoi(argv[2]);
  X = atoi(argv[3]);
}

int main(int argc, char *argv[]) {
  int i;
  get_args(argc, argv);

  array = malloc(sizeof(int) * N);
  Nr_aparitii = malloc(sizeof(int) * P);
  Suma_pozitii = malloc(sizeof(int) * P);

  for (i = 0; i < N; i++)
    array[i] = (i + 3) % 5;

  for (i = 0; i < N; i++)
    printf("%d ", array[i]);
  printf("\n");

  pthread_t tid[P];
  int thread_id[P];

  pthread_barrier_init(&barrier, NULL, P);

  for (i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (i = 0; i < P; i++)
    pthread_join(tid[i], NULL);

  pthread_barrier_destroy(&barrier);

  free(array);
  free(Nr_aparitii);
  free(Suma_pozitii);
  return 0;
}
