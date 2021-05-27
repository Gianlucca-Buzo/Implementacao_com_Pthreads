#include <stdio.h>
#include <malloc.h>
#include "pokeLib.h"
#include <stdlib.h>
#include <iostream>
#include <time.h> //clock(), CLOCKS_PER_SEC e clock_t 

void *fibo(void *dta);
void *mergeSort(void *dta);
#define MAX 3

int main()
{

    clock_t t; //variável para armazenar tempo

    int escolha, termino, contador;
    int n, *r, id;
    Atrib escalonamento;
    int *vetor = (int *)malloc((MAX + 2) * (sizeof(int)));
    vetor[0] = 0;
    vetor[1] = MAX;
    printf("----Selecione o código----\n");
    printf("- 1 - Fibonacci\n");
    printf("- 2 - Merge Sort\n");
    printf("--------------------------\n");
    scanf("%d", &escolha);
    t = clock(); //armazena tempo
    switch (escolha)
    {
    case 1:
        start(4, 1);
        //n = 3;
        printf("Digite o Valor do Fibonacci a ser calculado: ");
        scanf("%d", &n);
        escalonamento.p = 0;
        escalonamento.c = n;
        id = spawn(&escalonamento, fibo, &n);
        sync(id, (void **)&r);
        finish();
        //printf("fim");
        printf("Fibonacci (%d) = %d \n", n, *r);
        free(r); //adicionei essa linha
        break;
    case 2:
        for (int i = 2; i < MAX + 2; i++)
        {
            vetor[i] = rand() % 100;
        }

        for (int i = 2; i < MAX + 2; i++)
        {
            printf("%d, ", vetor[i]);
        }
        printf("\n");

        start(2, 0);

        escalonamento.p = 0;
        escalonamento.c = 0;

        id = spawn(&escalonamento, mergeSort, (void*)vetor);
        printf("spaw ID main: %d \n", id);
        sync(id, (void **)&r);
        finish();

        //mergeSort(vetor);
        printf("\n\n");
        if (MAX%2 ==0){
            contador = 2;
            termino = MAX+2;
        }
        else{
            contador = 3;
            termino = MAX +3;
        }
        for (contador; contador < termino; contador++)
        {
            printf("%d, ", vetor[contador]);
        }
        printf("\n");
        free(vetor);
       // free(r);
        break;
    default:
        printf("Voce digitou errado, seu bobao :(\n");
        break;
    }

    t = clock() - t; //tempo final - tempo inicial
     printf("Tempo de execucao: %lf", ((double)t)/((CLOCKS_PER_SEC/1000))); //conversão para double
    return 0;
}

void *fibo(void *dta)
{
    int *n = (int *)dta; // Esta linha foi modificada de *dta para dta
    int *n1, *n2, *r1, *r2;
    int *r = (int *)malloc(sizeof(int));
    struct Atrib a1, a2;
    int t1, t2;
    if (*n <= 2)
    {
        *r = 1;
    }
    else if (*n <= 40)
    {
        n1 = (int *)malloc(sizeof(int));
        r1 = (int *)malloc(sizeof(int));
        r2 = (int *)malloc(sizeof(int));
        *n1 = (*n) - 1;
        a1.p = 0;
        a1.c = *n1;
      //  printf("n1 %d ", *n1);
        t1 = spawn(&a1, fibo, (void *)n1);
        n2 = (int *)malloc(sizeof(int));
        *n2 = *n - 2;
        // printf("n2 %d ", *n2);
        a2.p = 0;
        a1.c = *n2;
        t2 = spawn(&a2, fibo, (void *)n2);
        sync(t1, (void **)&r1);
        sync(t2, (void **)&r2);
        // printf("%p\n %p ", r1, r2);
        *r = *r1 + *r2;
        free(r1);
        free(r2);
        free(n1);
        free(n2);
    }

    return (void *)r;
}

void *mergeSort(void *dta) //int *vetor, int posicaoInicio, int posicaoFim)
{
    
   // printf()
    int *aux = (int *)dta;
    int *vetor = &aux[2];//(int *)dta + (2 * sizeof(int));
    // int *segPosicao = aux++;
    //printf("aux 0 = %d aux 1 = %d\n",aux[0],aux[1]);
    //printf("vetor = %d %d %d %d %d",vetor[0], vetor[1], vetor[2], vetor[3], vetor[4]);
    int posicaoInicio = aux[0];
    int posicaoFim = aux[1];

    struct Atrib a1, a2;
    int t1, t2;
    int *r = (int *)malloc(sizeof(int));
    *r =1;

    int i, j, k, metadeTamanho, *vetorTemp;
    if (posicaoInicio == posicaoFim){
        return (void *)1;
    }
    metadeTamanho = (posicaoInicio + (posicaoFim)) / 2;
    aux[1] = metadeTamanho;
    t1 = spawn(&a1, mergeSort, dta);
    sync(t1, (void **)&r);

    aux[0] = metadeTamanho +1 ;
    aux[1] = posicaoFim;
    t2 = spawn(&a2, mergeSort, dta);
    sync(t2, (void **)&r);
    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    vetorTemp = (int *)malloc(sizeof(int) * (posicaoFim - posicaoInicio +1));

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
            if (j == posicaoFim + 1 )
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


    for (i = posicaoInicio; i <= posicaoFim; i++)//Provável problema
    {
        vetor[i] = vetorTemp[i - posicaoInicio];
    }

    free(vetorTemp);
    return (void *)1;
}