#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
  int n;
  int square;
  int cube;

} Arg;

void *calc(void *x) {
 Arg *p = x;

 p->square = p->n * p->n;
 p->cube = p->n * p->n * p->n;
 return 0;
}

 int main(void) {
  pthread_t tid;
  Arg       arg = {2, 0, 0};

  pthread_create(&tid, 0, calc, &arg);
  pthread_join(tid, 0);
  printf("%d %d\n", arg.square, arg.cube);

  return 0; 
 }
