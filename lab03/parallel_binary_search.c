#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define INSIDE -1  // daca numarul cautat este in interiorul intervalului
#define OUTSIDE -2 // daca numarul cautat este in afara intervalului

struct my_arg {
  int id;
  int N;
  int P;
  int number;
  int *left;
  int *right;
  int *keep_running;
  int *v;
  int *found;
  pthread_barrier_t *barrier;
};
void *f(void *arg) {
  stuct my_arg *data = (stuct my_arg *)arg;
  int size = *data->left - *data->right;
  int start = *data->left + data->id * size / data->P;
  int end = *data->left + (1 + data->id) * size / data->P;

  if (end > *data->right)
    end = *data->right;
  while (*data->keep_running) {
    if()
  }
}
// typedef struct my_arg my_arg;
void *f(void *arg) {
  struct my_arg *data = (struct my_arg *)arg;
  //*data ca eu am sus struct my_arg *data
  // si am data->id pt ca as avea (*data).id
  while (*data->keep_running) {
    int size = *data->right - *data->left;

    if (size <= 0) {
      *data->keep_running = 0;
      break;
    }

    int start = *data->left + data->id * (double)size / data->P;
    int end = *data->left + (int)((data->id + 1) * (double)size / data->P);
    if (end > *data->right)
      end = *data->right;

    data->found[data->id] = OUTSIDE;

    // TODO: implementati parallel binary search
    if (data->v[start] == data->number) {
      // prima pozitie din intervalul meu
      data->found[data->id] = start;
    } else if (data->v[end - 1] == data->number) {
      data->found[data->id] = end - 1;
    } else if (data->v[start] < data->number &&
               data->number < data->v[end - 1]) {
      data->found[data->id] = INSIDE;
    }
    pthread_barrier_wait(data->barrier);

    // acum doar un thread gestioneaza

    if (data->id == 0) {
      int count = 0;
      for (int i = 0; i < data->P; i++) {
        if (data->found[i] > INSIDE) {
          printf("l am gasit in tredul %d la pozitia %d", i, data->found[i]);
          *data->keep_running = 0;
          count = 1;
          break;
        } else if (data->found[i] == INSIDE) {
          *data->left = *data->left + i * (double)size / data->P;
          *data->right = *data->left + ((i + 1) * (double)size / data->P < size)
                             ? ((i + 1) * (double)size / data->P)
                             : size - 1;
          count = 1;
        }
      }
      if (count == 0) {
        // nu l am gasit in niciun interval
        printf("Number not found\n");
        *data->keep_running = 0;
      }
    }
    pthread_barrier_wait(data->barrier);
  }
  return NULL;
}
void display_vector(int *v, int size) {
  int i;

  for (i = 0; i < size; i++) {
    printf("%d ", v[i]);
  }

  printf("\n");
}

int main(int argc, char *argv[]) {
  int r, N, P, number, keep_running, left, right;
  int *v;
  int *found;
  void *status;
  pthread_t *threads;
  struct my_arg *arguments;
  pthread_barrier_t barrier;

  if (argc < 4) {
    printf("Usage:\n\t./ex N P number\n");
    return 1;
  }

  N = atoi(argv[1]);
  P = atoi(argv[2]);
  number = atoi(argv[3]);
  pthread_barrier_init(&barrier, NULL, P);
  keep_running = 1;
  left = 0;
  right = N;

  v = (int *)malloc(N * sizeof(int));
  threads = (pthread_t *)malloc(P * sizeof(pthread_t));
  arguments = (struct my_arg *)malloc(P * sizeof(struct my_arg));
  found = (int *)malloc(P * sizeof(int));

  for (int i = 0; i < N; i++) {
    v[i] = i * 2;
  }

  display_vector(v, N);

  for (int i = 0; i < P; i++) {
    arguments[i].id = i;
    arguments[i].N = N;
    arguments[i].P = P;
    arguments[i].number = number;
    arguments[i].left = &left;
    arguments[i].right = &right;
    arguments[i].keep_running = &keep_running;
    arguments[i].v = v;
    arguments[i].found = found;
    arguments[i].barrier = &barrier;

    r = pthread_create(&threads[i], NULL, f, &arguments[i]);

    if (r) {
      printf("Eroare la crearea thread-ului %d\n", i);
      exit(-1);
    }
  }

  for (int i = 0; i < P; i++) {
    r = pthread_join(threads[i], &status);

    if (r) {
      printf("Eroare la asteptarea thread-ului %d\n", i);
      exit(-1);
    }
  }

  free(v);
  free(threads);
  free(arguments);
  pthread_barrier_destroy(&barrier);
  return 0;
}
