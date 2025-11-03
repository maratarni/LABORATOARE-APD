#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
    schelet pentru exercitiul 5
*/
int *arr;
int array_size;
int num_threads;

typedef struct {
  int start;
  int end;
} ThreadData;
//am nevoie de un struct ca sa trimit mai multe date la thread
//functie pe care o va rula fiecare thread
void *thread_function(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  for (int i = data->start; i < data->end; i++){
    arr[i] += 100;
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Specificati dimensiunea array-ului si numarul de thread-uri\n");
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

  // TODO: aceasta operatie va fi paralelizata cu num_threads fire de executie
  

  pthread_t threads[num_threads];//tredurile noastre 
  ThreadData thread_data[num_threads];//datele pentru fiecare thread

int chunk = array_size / num_threads;
int remainder = array_size % num_threads; // dacă nu se împarte exact

int current = 0;
for (int i = 0; i < num_threads; i++) {
    int start = current;
    int end = start + chunk + (i < remainder ? 1 : 0); // distribuim echitabil

    thread_data[i].start = start;
    thread_data[i].end = end;

    pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
    current = end;
}

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
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
