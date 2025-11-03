#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int N;
int P;
int **a;
int **b;
int **c;

pthread_barrier_t barrier;
pthread_mutex_t lock;

void *thread_function(void *arg) {
  int thread_id = *(int *)arg;
  int start = thread_id * (double)N / P;
  int end = (thread_id + 1) * (double)N / P;
  if (end > N)
    end = N;

  int i, j, k;
  int sum_partiala;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      sum_partiala = 0;
      for (k = start; k < end; k++) {
        sum_partiala += a[i][k] * b[k][j];
      }
      pthread_mutex_lock(&lock);
      c[i][j] += sum_partiala;
      pthread_mutex_unlock(&lock);
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
    printf("Eroare la malloc!\n");
    exit(1);
  }

  int i, j;
  for (i = 0; i < N; i++) {
    a[i] = malloc(sizeof(int) * N);
    b[i] = malloc(sizeof(int) * N);
    c[i] = malloc(sizeof(int) * N);

    if (a[i] == NULL || b[i] == NULL || c[i] == NULL) {
      printf("Eroare la malloc!\n");
      exit(1);
    }
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      c[i][j] = 0;
      a[i][j] = 7 + (i + j) % 100;
      b[i][j] = 50 + (i + j) % 100;
    }
  }
}

void print_matrix(int **mat) {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      printf("%d\t", mat[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int i;
  double t1 = 5.920; // timp secvential (exemplu)

  //struct timespec start, end;

  get_args(argc, argv);
  init();
  pthread_barrier_init(&barrier, NULL, P);
  pthread_mutex_init(&lock, NULL);

  pthread_t *tid = malloc(sizeof(pthread_t) * P);
  int *thread_id = malloc(sizeof(int) * P);

  //clock_gettime(CLOCK_MONOTONIC, &start);

  for (i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }

 // clock_gettime(CLOCK_MONOTONIC, &end);

  //double Tp = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

//   printf("runtime = %.6f s\n", Tp);

//   double acceleratia = t1 / Tp;
//   printf("acceleratia = %.6f\n", acceleratia);

//   double eficienta = acceleratia / P;
//   printf("eficienta = %.6f\n", eficienta);

  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&lock);

  // eliberare memorie
  for (i = 0; i < N; i++) {
    free(a[i]);
    free(b[i]);
    free(c[i]);
  }
  free(a);
  free(b);
  free(c);
  free(tid);
  free(thread_id);

  return 0;
}

// cu N= 100
// p = 1 -> real =0 m 0.3242 s
// p = 2 -> real = 0 m 1.491 s
// p = 3 -> real = 0 m 0.991 s
// p = 4 -> real = 0 m 0.811 s 
// de unde nu se observa scalabilitatea = nr de threduri conoduce la un timp mai CLOCK_MONOTONIC

// timpul secevntial este   0 m 0.004 

// speedup(acceleratia) = timp_secvential/timp_paralel = 0.0123 -> abordarea secventiala e mai rapida in acest caz

// eficienta = acceleratie/p = 0.0123