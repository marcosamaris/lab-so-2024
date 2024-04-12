/*
O problema do produtor e consumidor (também conhecido como o problema do buffer limitado), consiste em um conjunto de processos que compartilham um mesmo buffer.
Os processos chamados produtores põem informação no buffer. Os processos chamados consumidores retiram informação deste buffer. Quando o buffer estiver cheio, os
produtores devem aguardar por posições disponíveis para inserir os dados produzidos. Da mesma forma, quando o buffer estiver vazio os consumidores devem aguardar
por dados para serem consumidos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define PR 1    //número de produtores
#define CN 1    // número de consumidores
#define N 5     //tamanho do buffer

void *produtor(void *meuid);
void *consumidor(void *meuid);

sem_t empty;    // semáforo que controla as posições livres
sem_t full;     // semáforo que controla as posições ocupadas
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;    // lock de acesso à região crítica

int index_in = 0;      // inicializando índice de inserção
int index_out = 0;     // inicializando índice de remoção
int buffer[N] = {0};   // declarando e inicializando o vetor do buffer com 0 em todas as posições

void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  sem_init(&empty, 0, N);     // inicializando o semáforo para analisar as posições do buffer que estão livres
  sem_init(&full, 0, 0);      // inicializando o semáforo para analisar as posições do buffer que estão ocupadas
  printf("BUFFER INICIALIZADO!\n");
  
  pthread_t tPid[PR];

  for (i = 0; i < PR; i++) {
    id = (int *)malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tPid[i], NULL, produtor, (void *)(id));

    if (erro) {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++) {
    id = (int *)malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *)(id));

    if (erro) {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  sem_destroy(&empty);    // destruíndo semáforo de posições livres
  sem_destroy(&full);     // destruíndo semáforo de posições ocupadas
  pthread_join(tPid[0], NULL);
}

void *produtor(void *pi) {
  int item, value;
  while (1) {
    item = rand()%5;     // produz dado

    sem_wait(&empty);    // decrementa semáforo de posições livres (down)
      pthread_mutex_lock(&mutex);     // produtor pega lock de acesso à região crítica
        buffer[index_in] = item;      // buffer recebe dado
        printf("Produtor   está inserindo o item %d no buffer[%d]\n", item, index_in);
        index_in = (index_in+1)%N;    // cálculo do índice do buffer em que o dado será inserido
      pthread_mutex_unlock(&mutex);   // produtor libera lock de acesso à região crítica
    sem_post(&full);    // incrementa semáforo de posições ocupadas

    sem_getvalue(&full, &value);    // verifica o valor do semáforo de posições ocupadas
    if (value == N) {               // se semáforo igual a N
      printf("BUFFER CHEIO!\n");    // buffer está cheio
    }

    sleep(rand()%3);
  }
  pthread_exit(0);
}

void *consumidor(void *pi) {
  int item, value;
  while (1) {

    sem_wait(&full);    // decrementa semáforo de posições ocupadas (down)
      pthread_mutex_lock(&mutex);     // consumidor pega lock de acesso à região crítica
        item = buffer[index_out];     // item recebe dado do buffer
        printf("Consumidor está removendo o item %d do buffer[%d]\n", item, index_out);
        index_out = (index_out+1)%N;  // cálculo do índice do buffer em que dado será removido
      pthread_mutex_unlock(&mutex);   // consumidor libera lock de acesso à região crítica
    sem_post(&empty);    // incrementa semáforo de posições livres (up)

    sem_getvalue(&full, &value);    // verifica o valor do semáforo de posições ocupadas
    if (value == 0) {               // se semáforo igual a 0
      printf("BUFFER VAZIO!\n");    // buffer está vazio
    }

    sleep(rand()%6);
  }
  pthread_exit(0);
}
