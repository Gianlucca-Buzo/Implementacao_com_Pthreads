#include <stdio.h>
#include <malloc.h>
#include "pokeLib.h"

void *fibo(void *dta);

int main()
{
    int escolha;
    printf("----Selecione o código----\n");
    printf("- 1 - Fibonacci\n");
    printf("- 2 - Merge Sort\n");
    printf("--------------------------\n");
    scanf("%d", &escolha);

    switch (escolha)
    {
    case 1:
        int n, *r, tId;
        struct Atrib a;
        start(4, 0);
        //n = 3;
        printf("Digite o Valor do Fibonacci a ser calculado: ");
        scanf("%d", &n);
        a.p = 0;
        a.c = n;
        tId = spawn(&a, fibo, &n);
        sync(tId, (void **)&r);
        finish();
        printf("fim");
        printf("Fibonacci (%d) = %d \n", n, *r);
        free(r); //adicionei essa linha
        break;
    case 2:
        break;
    default:
        printf("Voce digitou errado, seu bobao :(\n");
        break;
    }

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
    else
    {
        n1 = (int *)malloc(sizeof(int));
        r1 = (int *)malloc(sizeof(int));
        r2 = (int *)malloc(sizeof(int));
        *n1 = (*n) - 1;
        a1.p = 0;
        a1.c = *n1;
        t1 = spawn(&a1, fibo, (void *)n1);
        n2 = (int *)malloc(sizeof(int));
        *n2 = *n - 2;
        a2.p = 0;
        a1.c = *n2;
        t2 = spawn(&a2, fibo, (void *)n2);
        sync(t1, (void **)&r1);
        sync(t2, (void **)&r2);
        printf("%p\n %p ", r1, r2);
        *r = *r1 + *r2;
        free(r1);
        free(r2);
        free(n1);
        free(n2);
    }
    return r;
}
