// Marcella Pantarotto (13/0143880)

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

#define PR 1  //número de produtores
#define CN 1  // número de consumidores
#define N 5   //tamanho do buffer

void *produtor(void *meuid);
void *consumidor(void *meuid);
void print_buffer();
int produce_item();
void insert_data(int data, int i);
void remove_data(long int i);

int buffer[N] = {0};    // declarando e inicializando o vetor do buffer com 0 em todas as posições

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t produtor_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumidor_cond = PTHREAD_COND_INITIALIZER;

int count = 0;          // contador de posições
int index_insert = -1;   // índice do produtor
int index_remove = -1;   // índice do consumidor

void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  printf("START ");
  print_buffer();

  pthread_t tPid[PR];

  for (i = 0; i < PR; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tPid[i], NULL, produtor, (void *)(id));

    if (erro) {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *)(id));

    if (erro) {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_join(tPid[0], NULL);  
}

void *produtor(void *pi) {
  int item;

  while (1) {
    sleep(rand() % 2);
    item = produce_item();

    pthread_mutex_lock(&mutex);   // produtor pega o lock do buffer
      while (count == N) {  // verifica se o buffer está cheio
        printf("Buffer está cheio!\n\n");
        pthread_cond_wait(&produtor_cond, &mutex);  // adormece o produtor
      }

      index_insert = (index_insert + 1) % N;    // cálculo do índice do array circular
      insert_data(item, index_insert);      // inserir dado do buffer
      printf("PRODUTOR está produzindo conteúdo\n");
      
      count += 1;
      print_buffer();
      
      if (count == 1){  // verifica que o buffer não está vazio para acordar o consumidor
        printf("Acorda consumidor!\n\n");
        pthread_cond_signal(&consumidor_cond);  // como só tem 1 consumidor, não há problema usar signal
      } 
    pthread_mutex_unlock(&mutex);   // produtor solta o lock do buffer
    sleep(rand() % 2);
    // sleep((rand() % 5) + 5);     // teste -> alternar com consumidor
  }
  pthread_exit(0);
}

void *consumidor(void *pi) {
  while (1) {
    sleep(rand() % 2);

    pthread_mutex_lock(&mutex);   // consumidor pega o lock do buffer
      while (count == 0) {    // verifica se o buffer está vazio
        printf("Buffer está vazio!\n\n");
        pthread_cond_wait(&consumidor_cond, &mutex);    // adormece o consumidor
      }
      index_remove = (index_remove + 1) % N;    // cálculo do índice do array circular
      remove_data(index_remove);    // remover dado do buffer
      printf("CONSUMIDOR está consumindo conteúdo\n");

      count -= 1;
      print_buffer();

      if (count == N - 1){    // verifica que o buffer não está cheio para acordar o produtor
        printf("Acorda produtor!\n\n");
        pthread_cond_signal(&produtor_cond);    // como só tem 1 produtor, não há problema usar signal
      }
    pthread_mutex_unlock(&mutex);   // consumidor solta o lock do buffer
    // sleep(rand() % 2);
    sleep((rand() % 5) + 1);     // teste -> alternar com produtor
  }
  pthread_exit(0);
}

void print_buffer() {
  printf("BUFFER:\n");
  for (size_t i = 0; i < N; i++) {
    printf("buffer[%ld] = %d\n", i, buffer[i]);
  }
  printf("\n");
}

int produce_item(){
  return 1;
}

void insert_data(int data, int index){
  buffer[index] = data;
}

void remove_data(long int index){
  buffer[index] = 0;
}
