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

int quantidadeProcessadoresVirtuais = 0;
pthread_t *processadoresVirtuais;
list<trabalho *> prontos, terminados;
list<trabalho *>::iterator it;
pthread_mutex_t mutexProntos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTerminados = PTHREAD_MUTEX_INITIALIZER;
bool finaliza = false;
int idTrabalhoAtual = 1;
int opEscalonamento = 0;

trabalho *pegaUmTrabalhoPorID(int id, list<trabalho *> lista)
{
  if (lista.empty())
  {
    return NULL;
  }
  trabalho *retorno;
  for (it = lista.begin(); it != lista.end(); ++it)
  {
    if (*it == NULL)
    {
      return NULL;
    }
    printf("aaaaa %d\n", (*it)->idTrabalho);
    if ((*it)->idTrabalho == id)
    {
      retorno = *it;
      lista.erase(it);
      return retorno;
    }
  }
  return NULL;
}

trabalho *pegaUmTrabalho(list<trabalho *> lista)
{
  if (lista.empty())
  {
    return NULL;
  }
  trabalho *trabalhoRetorno = lista.front();
  lista.pop_front();
  return trabalhoRetorno;
}

void armazenaResultados(trabalho *trabalhoResultado, void *resultadoT)
{
  trabalhoResultado->resultado = resultadoT;
  switch (opEscalonamento)
  {
  case 1:
    terminados.push_front(trabalhoResultado); //pilha
    break;
  default:
    terminados.push_back(trabalhoResultado); //fila
    break;
  }
}

void executa(trabalho *trabalhoAtual)
{
  void *resultado;
  resultado = trabalhoAtual->funcao(trabalhoAtual->parametrosFuncao);
  pthread_mutex_lock(&mutexTerminados);
  armazenaResultados(trabalhoAtual, resultado);
  pthread_mutex_unlock(&mutexTerminados);
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

int start(int m, int escalonamento)
{
  /*Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais,
  indicados pelo parâmetro m. O retorno 0 (zero) indica falha na instanciação dos
  processadores virtuais.Um valor maior que 0 indica criação bem sucedida.*/
  //printf("Start %d\n", m);
  quantidadeProcessadoresVirtuais = m;
  opEscalonamento = escalonamento;
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
  pthread_mutex_lock(&mutexProntos);
  novoTrabalho->idTrabalho = idTrabalhoAtual;
  idTrabalhoAtual++; //Incrementa a variável global que faz a contagem dos valores dos IDs
  novoTrabalho->funcao = t;
  novoTrabalho->parametrosFuncao = dta;
  switch (opEscalonamento)
  {
  case 1:
    prontos.push_front(novoTrabalho); //pilha
    break;
  default:
    prontos.push_front(novoTrabalho); //fila
    break;
  }
  // printf("spaw lib: %d \n", novoTrabalho->idTrabalho);
  pthread_mutex_unlock(&mutexProntos);

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
  //printf("tId: %d: \n", tId);

  pthread_mutex_lock(&mutexTerminados);
  aux = pegaUmTrabalhoPorID(tId, terminados); // Retorna o trabalho ou null se não encontrar
  pthread_mutex_unlock(&mutexTerminados);
  if (aux == NULL)
  {
    pthread_mutex_lock(&mutexProntos);
    aux = pegaUmTrabalhoPorID(tId, prontos);
    pthread_mutex_unlock(&mutexProntos);
    if (aux != NULL)
    {
      executa(aux);
    }
    else
    {
      pthread_mutex_lock(&mutexProntos);
      aux = pegaUmTrabalho(prontos);
      pthread_mutex_unlock(&mutexProntos);

      if (aux != NULL)
      {
        executa(aux);
      }
      else
      {
        return 0;
      }
    }

    *res = aux->resultado;
    return 1;
    printf("Sync\n");
  }
}