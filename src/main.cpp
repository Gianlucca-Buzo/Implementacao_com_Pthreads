#include <stdio.h>
#include <malloc.h>
#include "pokeLib.h"

void* multiplicaPor10 (void* dta){
  int* n = (int*) dta;
  int *r = (int *) malloc (sizeof(int));
  *r = (*n) * 10;
  return r;
}

int main() {
  int id,r,param = 2;
  start(10);

  struct Atrib a;
	a.p = 0;
	a.c = 0;
	id = spawn(&a, multiplicaPor10, &param);
  printf("spaw ID main: %d \n", id);
  sync(id, (void**) &r);
  finish();

  printf("R main : %d\n", r);

  return 0;
}