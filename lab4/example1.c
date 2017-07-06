#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


typedef struct {
   int square;
   int cube;
} result;

void *calc(void *x) {
   int n = *(int*) x;
   result *p = malloc(sizeof(result));

   if (p==0)
      return 0;
 
   p->square = n*n;
   p->cube = n*n*n;
 
   return p;
}

 int main(void) {
  pthread_t tid;
  int       n = 2;
  result *retval;

  pthread_create(&tid, 0, calc, &n);
  pthread_join(tid, (void **)&retval);
  printf("%d %d\n", retval->square, retval->cube);
  free(retval);

  return 0; 
 }

