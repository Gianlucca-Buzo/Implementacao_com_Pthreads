#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <iostream>
#include <list>
#include <iterator>
using namespace std;

struct trabalho
{
  int idTrabalho;          // Id do trabalho
  void *(*funcao)(void *); // Função que vai ser executada
  void *parametrosFuncao;  // ParâmetroS de entrada para a função
  void *resultado;         // Retorno da função
};

static int quantidadeProcessadoresVirtuais = 0;
static pthread_t *processadoresVirtuais;
list<trabalho *> prontos, terminados;
list<trabalho *>::iterator it;
pthread_mutex_t mutexTiraDaLista, mutexColocaNaLista, mutexIncrementaId = PTHREAD_MUTEX_INITIALIZER;
bool finaliza = false;
int idTrabalhoAtual = 1;

trabalho *pegaUmTrabalhoPorID(int id, list<trabalho *> lista)
{
  trabalho *retorno;
  pthread_mutex_lock(&mutexTiraDaLista);
  for (it = lista.begin(); it != lista.end(); ++it)
  {
    if ((*it)->idTrabalho == id)
    {
      retorno = *it;
      lista.erase(it);
      pthread_mutex_unlock(&mutexTiraDaLista);
      return retorno;
    }
  }
  pthread_mutex_unlock(&mutexTiraDaLista);
  return NULL;
}

trabalho *pegaUmTrabalho(list<trabalho *> lista)
{
  pthread_mutex_lock(&mutexTiraDaLista);
  trabalho *trabalhoRetorno = lista.front();
  lista.pop_front();
  pthread_mutex_unlock(&mutexTiraDaLista);
  return trabalhoRetorno;
}

void armazenaResultados(trabalho *trabalhoResultado, void *resultadoT)
{
  trabalhoResultado->resultado = resultadoT;
  pthread_mutex_lock(&mutexColocaNaLista);
  terminados.push_back(trabalhoResultado);
  pthread_mutex_unlock(&mutexColocaNaLista);
}

void executa(trabalho *trabalhoAtual)
{
  void *resultado;
  resultado = trabalhoAtual->funcao(trabalhoAtual->parametrosFuncao);
  armazenaResultados(trabalhoAtual, resultado);
}

void *criaProcessadorVirtual(void *dta)
{
  void *resultado;
  trabalho *trabalhoAtual;
  while (!finaliza)
  {
    if (!prontos.empty())
    {
      trabalhoAtual = pegaUmTrabalho(prontos);
      executa(trabalhoAtual);
    }
  }
}

int start(int m)
{
  /*Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais,
  indicados pelo parâmetro m. O retorno 0 (zero) indica falha na instanciação dos
  processadores virtuais.Um valor maior que 0 indica criação bem sucedida.*/
  printf("Start %d\n", m);
  quantidadeProcessadoresVirtuais = m;
  processadoresVirtuais = (pthread_t *)malloc(m * sizeof(pthread_t));
  for (int i = 0; i < m; i++)
  {
    int threadCriada = pthread_create(&processadoresVirtuais[i], NULL, criaProcessadorVirtual, NULL); //apenas cria uma thread, sem nenhum tipo de parâmetro
    if (threadCriada != 0)
    {           //se houve problema na criação de um thread
      return 0; //retorna falha na instanciação
    }
  }
  return 1; //criação de todas as threads de maneira bem sucedida
}
void finish(void)
{
  /*Esta primitiva é bloqueante, retornando após todos os processadores virtuais terem finalizado.*/
  finaliza = true;
  idTrabalhoAtual = 0;
  for (int i = 0; i < quantidadeProcessadoresVirtuais; i++)
  {
    pthread_join(processadoresVirtuais[i], NULL);
    printf("Finish\n");
  }
}

int spawn(struct Atrib *atrib, void *(*t)(void *), void *dta)
{
  /*A primitiva spawn lança a execução da tarefa descrita no ponteiro para função 
indicada pelo parâmetro t.O parâmetro para a função *t é descrito no parâmetro dta.
O parâmetro struct Atrib* atrib descreve os atributos a serem considerados no 
escalonamento da tarefa. A função retorna 0 (zero) em caso de falha na criação da 
tarefa ou um valor inteiro positivo maior que 0, considerado o identificador único 
da tarefa no programa. Caso NULL seja passado como endereço para atrib, devem ser 
considerados os valores default para os atributos.*/
  trabalho *novoTrabalho = (trabalho *)malloc(sizeof(trabalho));
  pthread_mutex_lock(&mutexIncrementaId);
  novoTrabalho->idTrabalho = idTrabalhoAtual;
  idTrabalhoAtual++; //Incrementa a variável global que faz a contagem dos valores dos IDs
  novoTrabalho->funcao = t;
  novoTrabalho->parametrosFuncao = dta;
  pthread_mutex_unlock(&mutexIncrementaId);
  //Adiciona elemento na lista prontos
  pthread_mutex_lock(&mutexColocaNaLista);
  prontos.push_back(novoTrabalho);
  printf("spaw lib: %d \n", novoTrabalho->idTrabalho);
  pthread_mutex_unlock(&mutexColocaNaLista);

  return novoTrabalho->idTrabalho;
}

int sync(int tId, void **res)
{
  /*A primitiva sync é bloqueante: a tarefa que invoca a primitiva sync informa qual tarefa, 
identificada no parâmetro tId, que deve ser sincronizada. O retorno da primitiva é 0 (zero)
em caso de falha ou 1 (um), em caso de sincronização bem sucedida. O parâmetro res contém,
como saída, o endereço de memória que contém os resultados de saída. Importante observar: uma
tarefa somente pode ser sincroniza uma única vez. Não é permitido múltiplos syncs de uma mesma
tarefa*/
  trabalho *aux;
  printf("tId: %d: \n", tId);

  aux = pegaUmTrabalhoPorID(tId, terminados); // Retorna o trabalho ou null se não encontrar
  if (aux == NULL)
  {
    aux = pegaUmTrabalhoPorID(tId, prontos);
    if (aux!= NULL){
      executa(aux);
    }
    else
    {
      aux = pegaUmTrabalho(prontos);
      if (aux!=NULL){
        executa(aux);
      }
      else
      {
        return 0;
      }
    }
  }
  return 1;
  printf("Sync\n");
}