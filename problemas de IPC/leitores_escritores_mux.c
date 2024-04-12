/*
Este problema é uma abstração do acesso à base de dados, onde não existe o perigo em termos diversos processos lendo concorrentemente (leitores), mas escrevendo ou 
mudando os dados (escritores) deve ser feito sob exclusão mútua para garantir consistência. Leitores e escritores é uma família de modelos de controle de concorrência 
em que leitores (entes que não alteram conteúdo) pode acessar concorrentemente os recursos (por exemplo, um banco de dados) e escritores (entes que alteram conteúdo) 
requerem acesso exclusivo. Usando como base o código disponibilizado neste tópico, resolva o problema de starvation dos escritores.
*/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define TRUE 1

#define NE 10 //numero de escritores
#define NL 10 //numero de leitores

// (se usar apenas um lock garante a exclusão mútua da região crítica por apenas 1 leitor ou 1 escritor)
pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;	// lock do banco de dados
pthread_mutex_t lock_nl = PTHREAD_MUTEX_INITIALIZER;	// lock de leitura
pthread_mutex_t lock_vez = PTHREAD_MUTEX_INITIALIZER;	// lock da vez que quem vai passar --> serve para os leitores e escritores competirem entre si pra pegar o lock do banco de dados

int num_leitores = 0;	 // contador de leitores que estão acessando o banco de dados para leitura

void *reader(void *arg); // thread de execução dos leitores
void *writer(void *arg); // thread de execução dos escritores
void read_data_base();	 // função usada pelo leitor para ler um dado na base de dados					// acessa região crítica
void use_data_read();	 // função usada pelo leitor para usar o dado lido								// não acessa região crítica
void think_up_data();	 // função usada pelo escritor para produzir um dado							// não acessa região crítica
void write_data_base();	 // função usada pelo escritor para escrever o dado produzido na base de dados	// acessa região crítica

int main(){
	pthread_t r[NL], w[NE]; // 2 vetores: 1 para leitores e 1 para escritores
	int i;
	int *id;
	/* criando leitores */
	for (i = 0; i < NL; i++)
	{
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&r[i], NULL, reader, (void *)(id));
	}
	/* criando escritores */
	for (i = 0; i < NE; i++)
	{
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&w[i], NULL, writer, (void *)(id));
	}
	pthread_join(r[0], NULL); // dá join em apenas 1 dos leitores, pois bloqueia a main e garante que as threads sempre executem
	return 0;
}

void *reader(void *arg)
{
	int i = *((int *)arg);
	while (TRUE){ /* repete para sempre */
		// região de exclusão mútua, entre os leitores
		pthread_mutex_lock(&lock_vez);				// escritor pega lock vez
			pthread_mutex_lock(&lock_nl);			// leitor pega o lock de leitura
				num_leitores++;						// incrementa contador de leitores acessando o banco de dados
				if (num_leitores == 1){				// teste para ver se é o primeiro leitor
					pthread_mutex_lock(&lock_bd); 	// se for o primeiro, fecha o lock do banco de dados
				}
			pthread_mutex_unlock(&lock_vez);		// escritor libera o lock vez		
		pthread_mutex_unlock(&lock_nl);				// leitor libera o lock de leitura

	
		read_data_base(i); 							/* acesso aos dados */

		// região de exclusão mútua, entre os leitores
		pthread_mutex_lock(&lock_nl);				// leitor pega o lock de leitura
			num_leitores--;							// decrementa contador de leitores acessando o banco de dados
			if (num_leitores == 0){					// teste para ver se é o último leitor
				pthread_mutex_unlock(&lock_bd);		// se for o último, libera o lock do banco de dados
			}
		pthread_mutex_unlock(&lock_nl);				// leitor libera o lock de leitura
		
		use_data_read(i);							/* região não crítica */	// fica fora do lock, pois não atrapalha o acesso aos dados
	}
	pthread_exit(0);
}

void *writer(void *arg){
	int i = *((int *)arg);
	while (TRUE){								/* repete para sempre */	
		think_up_data(i); 						/* região não crítica */		// fica fora do lock, pois não atrapalha o acesso aos dados
		pthread_mutex_lock(&lock_vez);			// escritor pega lock vez
			// o lock garante que mais de um escritor não acesse o banco de dados ao mesmo tempo, vai ter sempre no máximo 1 escritor na região crítica
			pthread_mutex_lock(&lock_bd);		// escritor pega o lock do banco de dado
				write_data_base(i); 			/* atualiza os dados */
			pthread_mutex_unlock(&lock_vez);	// escritor libera o lock vez
		pthread_mutex_unlock(&lock_bd);			// escritor libera o lock do banco de dados

	}
	pthread_exit(0);
}

void read_data_base(int i){
	printf("Leitor %d está lendo os dados! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5);
}

void use_data_read(int i){
	printf("Leitor %d está usando os dados lidos! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5);
}

void think_up_data(int i){
	printf("Escritor %d está pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i){
	printf("Escritor %d está escrevendo os dados! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5 + 15);
}