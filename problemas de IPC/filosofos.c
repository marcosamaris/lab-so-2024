/*
The problem is defined as follows: There are 5 philosophers sitting at a round table. Between each adjacent pair of philosophers is a chopstick.
In other words, there are five chopsticks. Each philosopher does two things: think and eat. The philosopher thinks for a while, and then stops
thinking and becomes hungry. When the philosopher becomes hungry, he/she cannot eat until he/she owns the chopsticks to his/her left and right.
When the philosopher is done eating he/she puts down the chopsticks and begins thinking again.
*/

// https://web.archive.org/web/20131208203319/http://web.eecs.utk.edu/~plank/plank/classes/cs560/560/notes/Dphil/lecture.html

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5					// quantidade de talheres e filósofos
#define ESQ(id) (id)		// talher da esquerda
#define DIR(id)	(id+1)%N	// talher da direita

#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2
int estados[N];

sem_t s[N];		// semáforo para os talheres
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *filosofos (void *arg);
void pega_talher (int n);		// funções executadas pelos filósofo
void devolve_talher (int n);	// funções executadas pelos filósofo
void comer (int n);		// funções executadas pelos filósofo

int main () {
	int i;
	int *id;
	//semaforo 
	for (i=0; i<N; i++) {
		sem_init(&s[i], 0, 0);		// inicialização semáforo, filosofo incrementa semáforo quando pega o talher
	}
	pthread_t r[N]; 

	//criacao das threads de filosofos
    for (i = 0; i < N ; i++) {
		id = (int *) malloc(sizeof(int));
		*id = i;
		pthread_create(&r[i], NULL, filosofos, (void*) (id));
	}

    pthread_join(r[0],NULL);
	return 0;
}

void *filosofos (void *arg) {
	int n = *((int *) arg);
	while(1) {
		//pensar
		printf("Filosofo %d pensando ...\n", n);
		sleep(3);
		
		pega_talher(n);
		//comer
		printf("\tFilosofo %d comendo ...\n", n);
		sleep(3);
        
		printf("\tFilosofo %d acabou de comer ...\n", n);
		devolve_talher(n);
	} 
}

void pega_talher (int n) {		// n = id do filófoso
	/*
	if(n/2 == 0){
		sem_wait(&s[DIR(n)]);
		sleep(1);
		sem_wait(&s[ESQ(n)]);
	}else{
		sem_wait(&s[ESQ(n)]);
		sleep(1);
		sem_wait(&s[DIR(n)]);
	} */

	pthread_mutex_lock(&mutex);		// pega lock para mudar estado
		estados[n] = FAMINTO;
		comer(n);
	pthread_mutex_unlock(&mutex);	// libera lock
	sem_wait(&s[n]);	// decrementa semáforo para talher ficar livre
}

void devolve_talher (int n) {
	pthread_mutex_lock(&mutex);		// pega lock para mudar estado
		estados[n] = PENSANDO;
		comer(ESQ(n));	// fazer vizinho comer
		comer(DIR(n));	// fazer vizinho comer
	pthread_mutex_unlock(&mutex);	// libera lock
}

void comer (int n) {
	// testar se pode comer
	if(estados[n] == FAMINTO && estados[ESQ(n)] != COMENDO && estados[DIR(n)] != COMENDO) {
		estados[n] = COMENDO;
		sem_post(&s[n]);	// incrementa semáforo pra indicar que pegou talher
	}
}
