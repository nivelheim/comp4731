#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define MAXARR 10

struct {
  int nused;
  int next_write;
  int next_read;
  int data[MAXARR];
} buf = {0, 0, 0, {0}};

pthread_cond_t prKey = PTHREAD_COND_INITIALIZER;
pthread_cond_t coKey = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* produce(void* arg) {
  int item = 0;
  int id = (int)(uintptr_t) arg;
  
  while(1) {
    pthread_mutex_lock(&mutex);
    while(buf.nused == MAXARR) {
      pthread_cond_wait(&prKey, &mutex);
      printf("producer in the buffer\n");  
    }

    buf.nused++;
    buf.data[buf.next_write] = item;
    item++;
  
    printf("P%d: %d --> [%d] thread used: %d\n", id, buf.data[buf.next_write], buf.next_write, buf.nused);
    buf.next_write = (buf.next_write + 1) % MAXARR;

    pthread_cond_signal(&coKey);
    pthread_mutex_unlock(&mutex);
    sleep(1);
  }

  pthread_exit(0);
}

void* consume(void* arg) {
  int i;
  int id = (int)(uintptr_t) arg;

  while(1) {
    pthread_mutex_lock(&mutex);

    while(buf.nused == 0) {
      pthread_cond_wait(&coKey, &mutex);
      printf("consumer wait");
    }

    buf.nused--;
    i = buf.next_read;
    
    printf("C%d: %d <-- [%d] thread used: %d\n", id, buf.data[i], i, buf.nused);
    
    buf.next_read = (buf.next_read+1) % MAXARR;
    pthread_cond_signal(&prKey);
    pthread_mutex_unlock(&mutex);

    sleep(1);
  }
  
  pthread_exit(0);
}


int main(void) {
  pthread_t pid1, pid2, pid3, pid4;

  pthread_create(&pid1, NULL, produce, (void*) 0);
  pthread_create(&pid2, NULL, produce, (void*) 1);
  pthread_create(&pid3, NULL, consume, (void*) 0);
  pthread_create(&pid4, NULL, consume, (void*) 1);
  

  pthread_exit(0);

  return 0;
}
