#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int P;
int N;
int sleep_data[9]; // max 8 threaduri
int done[9];       // pe care il fac 1 cand s a term un thread
// am deci N si toate duratele de sleep
pthread_mutex_t mutex;
void get_args(int argc, char **argv) {
  if (argc < 3) {
    printf("Numar insuficient de parametri: ./program P t1 t2...\n");
    exit(1);
  }
  // N = atoi(argv[1]);
  P = atoi(argv[1]);
  if (P < 2 || P > 8)
    exit(1);
  int i;
  for (i = 1; i < P; i++) {
    sleep_data[i] = atoi(argv[i + 1]);
    if (sleep_data[i] > 7)
      exit(1);
  }
}

void *thread_function(void *arg) {
  int i;
  int thread_id = *(int *)arg;
  if (thread_id != 0) {
    sleep(sleep_data[thread_id]);
    pthread_mutex_lock(&mutex);
    done[thread_id] = 1;
    pthread_mutex_unlock(&mutex);
  }

  else {
    int contor = 0;
    while (contor != P - 1) {
      pthread_mutex_lock(&mutex);
      for (i = 1; i < P; i++) {
        if (done[i] == 1) {
          printf("Thread-ul %d a terminat\n", i);
          done[i] = 2;
          contor++;
        }
      }
      pthread_mutex_unlock(&mutex);
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  get_args(argc, argv);

  int i;
  pthread_t tid[P];
  int thread_id[P];
  pthread_mutex_init(&mutex, NULL);

  for (i = 0; i < P; i++) {
    thread_id[i] = i;
    pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
  }

  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }
  pthread_mutex_destroy(&mutex);

  return 0;
}
