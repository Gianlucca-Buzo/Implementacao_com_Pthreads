#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include "pokeLib.h"

void* mergeSort( void* dta )//int *vetor, int posicaoInicio, int posicaoFim)
{
  void *aux = dta;
  int* posicaoInicio = (int*) dta;
  int* posicaoFim = (int*) dta + 4;
  //int len = sizeof(vetor)/sizeof(int);

  int i, j, k, metadeTamanho, *vetorTemp;
  if (posicaoInicio == posicaoFim)
    return;
  metadeTamanho = (posicaoInicio + posicaoFim) / 2;

  mergeSort(vetor, posicaoInicio, metadeTamanho);
  mergeSort(vetor, metadeTamanho + 1, posicaoFim);

  i = posicaoInicio;
  j = metadeTamanho + 1;
  k = 0;
  vetorTemp = (int *)malloc(sizeof(int) * (posicaoFim - posicaoInicio + 1));

  while (i < metadeTamanho + 1 || j < posicaoFim + 1)
  {
    if (i == metadeTamanho + 1)
    {
      vetorTemp[k] = vetor[j];
      j++;
      k++;
    }
    else
    {
      if (j == posicaoFim + 1)
      {
        vetorTemp[k] = vetor[i];
        i++;
        k++;
      }
      else
      {
        if (vetor[i] < vetor[j])
        {
          vetorTemp[k] = vetor[i];
          i++;
          k++;
        }
        else
        {
          vetorTemp[k] = vetor[j];
          j++;
          k++;
        }
      }
    }
  }
  for (i = posicaoInicio; i <= posicaoFim; i++)
  {
    vetor[i] = vetorTemp[i - posicaoInicio];
  }
  free(vetorTemp);
}

int main()
{
  void *dta, *aux;

  int *vetor = (int *)malloc(10 * (sizeof(int)));
  for (int i = 0; i < 10; i++)
  {
    vetor[i]= rand() % 10;
  }
  
  
  int n, *r, tId;
	struct Atrib a;
	start(4);
	//n = 3;
	printf("Digite o Valor do Fibonacci a ser calculado: ");
	scanf("%d",&n);
	a.p = 0;
	a.c = n;
	tId = spawn(&a, mergeSort, &n);
	sync(tId, (void **)&r);
	finish();
	printf("fim");
	printf("Fibonacci (%d) = %d \n", n, *r);
  
  return 0;
}