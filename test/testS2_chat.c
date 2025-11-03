#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

int N;
int P;
int sleep_time[9]; // max 8 threaduri 
pthread_mutex_t mutex;
int done[9]; //1 cand thread ul i a terminat 

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./merge N P X (N trebuie sa fie putere a lui 2)\n");
		exit(1);
	}
	P = atoi(argv[1]);
    if(P < 2 || P > 8)
    {
        exit(1);
    }
    for(int i=1; i < P; i++) //ultimele P-1 realizeaza un job abstract
    {
        sleep_time[i] = atoi(argv[i+1]);
        if(sleep_time[i] <1 || sleep_time[i] > 7)
        {
            printf("durata incorecta");
            exit(1);
        }
    }
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

    if(thread_id !=0)
    {
        sleep(sleep_time[thread_id]); // job abstract

        //trebuie sa imi scrie pe rand in vector, sa nu se suprapuna
        pthread_mutex_lock(&mutex);//sa nu se suprapuna scrierile 
        done[thread_id] = 1;
        pthread_mutex_unlock(&mutex);
    }
    else //daca este threadul 0 trebuie sa afiseze 
    {
        int printed = 0;
        //folosim while deoarece threadurile temina la momente de timp diferite 
        while(printed < P-1)
        {
            //mutex: race condition  sa scrie pe rand in vectorul done 
            pthread_mutex_lock(&mutex);
            for(int i =1; i < P; i++)
            {
                if(done[i] == 1)
                {
                    printf("Thread-ul %d a terminat\n", i);
                    done[i] = 2;
                    printed++;
                }
            }
            pthread_mutex_unlock(&mutex);
        }

    }
    
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);

	int i;
	int thread_id[P];
	pthread_t tid[P];
    pthread_mutex_init(&mutex, NULL);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	return 0;
}