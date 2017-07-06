#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* print(void *arg) {
  char *word = (char*) arg;
  while(1) {
    printf("%s\n", word);
    sleep(2);
  }

  pthread_exit(0);
}

int main(void) {
  pthread_t tid1, tid2;
  //int status;

  pthread_create(&tid1, NULL, print, "hello");
  pthread_create(&tid2, NULL, print, "world");
  
  while (1) {
  }
  //pthread_join(tid1, (void *) &status);
  //pthread_join(tid2, (void *) &status);
  return 0;
}
