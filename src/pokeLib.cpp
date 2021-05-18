#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <iostream>
#include <list>
#include <iterator>
using namespace std;

struct trabalho{
  int idTrabalho;          // Id do trabalho
  void *(*funcao)(void *); // Função que vai ser executada
  void *parametrosFuncao;  // ParâmetroS de entrada para a função
  void *resultado;         // Retorno da função
};

static int quantidadeProcessadoresVirtuais = 0;
static pthread_t *processadoresVirtuais;
list<trabalho*> prontos, terminados;
list<int>::iterator it;
pthread_mutex_t mutexTiraDaLista, mutexColocaNaLista = PTHREAD_MUTEX_INITIALIZER;
bool finaliza = false;

trabalho* pegaUmTrabalho(list<trabalho*> lista){
  pthread_mutex_lock( &mutexTiraDaLista );
  trabalho* trabalhoRetorno = lista.front();
  lista.pop_front();
  pthread_mutex_unlock( &mutexTiraDaLista );
  return trabalhoRetorno;
}

void armazenaResultados(trabalho *trabalhoResultado, void *resultadoT)
{ 
    trabalhoResultado->resultado = resultadoT;
    pthread_mutex_lock( &mutexColocaNaLista );
    terminados.push_back(trabalhoResultado);
    pthread_mutex_unlock( &mutexColocaNaLista );
}


void* criaProcessadorVirtual(void *dta){
  void* resultado;
  trabalho* trabalhoAtual;
  while(!finaliza){
    if(!prontos.empty()){
      trabalhoAtual = pegaUmTrabalho(prontos);
      resultado = trabalhoAtual->funcao(trabalhoAtual->parametrosFuncao);
      armazenaResultados(trabalhoAtual, resultado);
    }    
  
  }
}

int start (int m){
    /*Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais,
  indicados pelo parâmetro m. O retorno 0 (zero) indica falha na instanciação dos
  processadores virtuais.Um valor maior que 0 indica criação bem sucedida.*/
  printf("Start %d\n", m);
    quantidadeProcessadoresVirtuais = m;
    processadoresVirtuais = (pthread_t*) malloc(m * sizeof(pthread_t));
    for (int i = 0; i < m; i++){
        int threadCriada = pthread_create(&processadoresVirtuais[i], NULL, criaProcessadorVirtual, NULL); //apenas cria uma thread, sem nenhum tipo de parâmetro
        if (threadCriada != 0){ //se houve problema na criação de um thread
            return 0; //retorna falha na instanciação
        }
    }
    return 1; //criação de todas as threads de maneira bem sucedida
}



void finish(void){
  /*Esta primitiva é bloqueante, retornando após todos os processadores virtuais terem finalizado.*/
    finaliza = true;
    for (int i = 0; i < quantidadeProcessadoresVirtuais; i++)
    {
        pthread_join(processadoresVirtuais[i], NULL);
    }
  printf("Finish\n");
}

int spawn( struct Atrib* atrib, void *(*t) (void *), void* dta ){
  int* n = (int*) t(dta);
  printf("spaw lib: %d \n", *n);
  return *n;
}

int sync( int tId, void** res ){
  printf("Sync\n");
  tId += 2;
  *res = (void **)tId;
  return 1;
}