#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define INSIDE -1 // daca numarul cautat este in interiorul intervalului
#define OUTSIDE -2 // daca numarul cautat este in afara intervalului
#define min(a, b) (((a) < (b)) ? (a) : (b))

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

/*
void binary_search() {
	while (keep_running) {
		int mid = left + (right - left) / 2;
		if (left > right) {
			printf("Number not found\n");
			break;
		}

		if (v[mid] == number) {
			keep_running = 0;
			printf("Number found at position %d\n", mid);
		} else if (v[mid] > number) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
}
*/

void *f(void *arg)
{
	struct my_arg* data = (struct my_arg*) arg;

	while (*data->keep_running) {

		int size = *data->right - *data->left;

        if (size <= 0) { // interval invalid -> stop
            *data->keep_running = 0;
            break;
        }

        //calculeaza segmentul pentru thread ul curent 
        int start = *data->left + data->id * (double) size/data->P;
        int stop =  *data->left + min((data->id + 1) * (double) size/data->P, size);

        data->found[data->id] = OUTSIDE;

        //verifica pentru capete
        if(data->v[start] == data->number) {
            //e pe prima pozitie din intervalul meu 
            data->found[data->id] = start;
        } else if (data->v[stop - 1] == data->number) {
            // e pe ultima pozitie din intervalul meu 
            data->found[data->id] = stop-1;
        } else if(data->v[start] < data->number && data->number < data->v[stop - 1]) {
            //e in interiorul intervalului meu 
            data->found[data->id] = INSIDE;
        }

        pthread_barrier_wait(data->barrier);  
        /*
        💡 Explicație:
        Toate thread-urile trebuie să termine verificarea intervalelor ÎNAINTE
        ca thread-ul 0 să decidă noul interval de căutare.
        */

		// thread ul 0 coordoneaza
        if(data->id == 0){
            //thread ul 0 decide pe baza rezultatelor 
            int found_something = 0; //1 daca l am gasit sau avem interval inside
            for (int i = 0; i < data->P; i++) {
                //daca un thread a gasit numarul
                if (data->found[i] > INSIDE) {
                    printf("FOUND by thread %d at position %d\n", i, data->found[i]);
                    *data->keep_running = 0;
                    found_something = 1;
                    break;
                } 
                else 
                if(data->found[i] == INSIDE) 
                {
                    //acest thread contine intervalul in care este posibil sa se afle numarul   cautat
                    *data->left = *data->left + i * (double) size / data->P + 1;
                    data->right = *data->right + min((i+1)(double) size / data->P, size) -1; 
                    found_something = 1;  
                }
            }
            if (found_something == 0) {
            // nu l am gasit in niciun interval
            printf("Number not found\n");
            *data->keep_running = 0;
        }
        }
        // ===== Bariera 2 =====
        pthread_barrier_wait(data->barrier);
        /*
        💡 Explicație:
        Toate thread-urile trebuie să aștepte update-ul lui left/right
        făcut de thread-ul 0 înainte să repornească bucla.
        Dacă n-am pune bariera, unele thread-uri ar vedea interval vechi.
        */
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


int main(int argc, char *argv[])
{
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

	v = (int*) malloc(N * sizeof(int));
	threads = (pthread_t*) malloc(P * sizeof(pthread_t));
	arguments = (struct my_arg*) malloc(P * sizeof(struct my_arg));
	found = (int*) malloc(P * sizeof(int));

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