#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h> 

#define NITEMS 100

struct {
  int next_write;
  int next_read;
  int data[NITEMS];
} buf = {0, 0, {0}};

sem_t avail;
sem_t mutex;
sem_t used;

void* consume(void* args);
void* produce(void* args);


void* consume(void* args) {
  int id = (int)(uintptr_t) args;
  int i;

  while (1) {
    sem_wait(&used);
    sem_wait(&mutex);

    i = buf.next_read;

    printf("C%d: %d <-- [%d]\n", id, buf.data[i], i);
    buf.next_read = (buf.next_read + 1) % NITEMS;

    sem_post(&mutex);
    sem_post(&avail);
  
    sleep(2);
  }
    
  pthread_exit(0);
  
}

void* produce(void* args) {
  int id = (int)(uintptr_t) args;
  int i;
  int value = 0;

  while(1) {
    sem_wait(&avail);
    sem_wait(&mutex);

    i = buf.next_write;

    buf.data[i] = value++;

    printf("P%d: %d --> [%d]\n", id, buf.data[i], i);
    buf.next_write = (buf.next_write + 1) % NITEMS;

    sem_post(&mutex);
    sem_post(&used);

    sleep(2);
  }
  
  pthread_exit(0);

}

int main(void) {
  pthread_t tid1, tid2, tid3, tid4;

  sem_init(&avail, 0, NITEMS);
  sem_init(&used, 0, 0);
  sem_init(&mutex, 0, 1);

  pthread_create(&tid1, NULL, produce, (void*)0);
  pthread_create(&tid2, NULL, produce, (void*)1);
  pthread_create(&tid3, NULL, consume, (void*)0);
  pthread_create(&tid4, NULL, consume, (void*)1);

  /*
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
  */
  pthread_exit(0);

  return 0;
}
