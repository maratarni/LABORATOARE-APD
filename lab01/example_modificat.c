#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2

void *f1(void *arg) {
    long id = *(long *)arg;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %ld (f1): iteratia %d\n", id, i);
        usleep(200000); // mică pauză pentru a vedea intercalarea
    }
    pthread_exit(NULL);
}

void *f2(void *arg) {
    long id = *(long *)arg;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %ld (f2): iteratia %d\n", id, i);
        usleep(150000);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    void *status;
    long id1 = 1, id2 = 2;

    // Creează thread-ul 1
    if (pthread_create(&t1, NULL, f1, &id1)) {
        printf("Eroare la crearea thread-ului 1\n");
        exit(-1);
    }

    // Creează thread-ul 2
    if (pthread_create(&t2, NULL, f2, &id2)) {
        printf("Eroare la crearea thread-ului 2\n");
        exit(-1);
    }

    // Așteaptă finalizarea celor două thread-uri
    pthread_join(t1, &status);
    pthread_join(t2, &status);

    printf("Am terminat!\n");
    return 0;
}
