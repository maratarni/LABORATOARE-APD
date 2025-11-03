#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
/*
        schelet pentru exercitiul 5
*/
typedef struct {
  int start;
  int end;
} data_thread;
int *arr;
int array_size;
int num_threads;

void *function_thread(void *arg) {
  data_thread *dataThread = (data_thread *)arg;

  for (int i = dataThread->start; i < dataThread->end; i++) {
    arr[i] = arr[i] + 100;
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr,
            "Specificati dimensiunea array-ului si numarul de thread-uri\n");
    exit(-1);
  }

  array_size = atoi(argv[1]);
  num_threads = atoi(argv[2]);

  arr = malloc(array_size * sizeof(int));
  for (int i = 0; i < array_size; i++) {
    arr[i] = i;
  }

  for (int i = 0; i < array_size; i++) {
    printf("%d", arr[i]);
    if (i != array_size - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }

  pthread_t threads[num_threads];
  data_thread DataThread[num_threads];
  int start, end, current = 0;

  int chunk = array_size / num_threads;
  int rest = array_size % num_threads;

  current = 0;
  for (int i = 0; i < num_threads; i++) {
    start = current;
    end = start + chunk + (i < rest ? 1 : 0);

    DataThread[i].start = start;
    DataThread[i].end = end;

    current = end;

    pthread_create(&threads[i], NULL, function_thread, &DataThread[i]);
  }
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  //   gettimeofday(&end, NULL);

  //   double elapsed =
  //       (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

  // printf("Timp executie paralela: %.6f secunde\n", elapsed);
  for (int i = 0; i < array_size; i++) {
    printf("%d", arr[i]);
    if (i != array_size - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }

  return 0;
}