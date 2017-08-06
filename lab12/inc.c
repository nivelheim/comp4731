#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <limits.h>


#define SHMEM "/sharedMem"
#define NUSED 25

typedef struct{
  sem_t lock;
  unsigned long value;
} counter;

int main(void) {
  counter *shared_data;
  int fd;

  size_t i;

  fd = shm_open(SHMEM, O_RDWR, 0777);

  if (fd < 0) {
    exit(-1);
  }

  shared_data = mmap(NULL, sizeof(counter), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_data == MAP_FAILED) {
    exit(-1);
  }

  for (i=0; i<NUSED; i++) {
    if (sem_wait(&shared_data->lock) == -1) {
      exit(-1);
    }
  

    fprintf(stdout, "2:Current value:\t%lu\n", shared_data->value);
    shared_data->value++;
    fprintf(stdout, "2:New value:\t%lu\n", shared_data->value);
    sleep(1);

    if (sem_post(&shared_data->lock) == -1) {
      exit(-1);
    }  
  }
  
  return 0;
}
