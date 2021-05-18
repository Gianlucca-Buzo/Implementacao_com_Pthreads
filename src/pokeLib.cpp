#include <stdio.h>

int start (int m){
  printf("Start %d\n", m);
  return m;
}
void finish(void){
  printf("Finish\n");
}
int spawn( struct Atrib* atrib, void *(*t) (void *), void* dta ){
  int* n = (int*) t(dta);
  
  printf("spaw lib: %d \n", *n);
  return *n;
}
int sync( int tId, void** res ){
  printf("Sync\n");
  *res = (int *)(tId +1);
  return 1;
}