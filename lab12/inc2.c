#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <limits.h>

#define SEM "/sharedSem"
#define MEM "/sharedMem"
#define NUSED 100000

int main(void) {
  int *shared_data;
  sem_t *lock;
  int fd;
  int shared_size;

  size_t i;

  shared_size = sizeof(int);
  fd = shm_open(MEM, O_RDWR, 0777);

  if (fd < 0) {
    perror("shm_open");
    exit(-1);
  }

  shared_data = mmap(NULL, shared_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_data == MAP_FAILED) {
    perror("mmap");
    exit(-1);
  }

  lock = sem_open(SEM, 0);
  if (lock == SEM_FAILED) {
    perror("sem_open");
    exit(-1);
  }
  
  for (i=0; i<NUSED; i++) {
    if (sem_wait(lock) == -1) {
      exit(-1);
    }
  
    fprintf(stdout, "2:Current value:\t%d\n", *shared_data);
    ++*shared_data;
    fprintf(stdout, "2:New value:\t%d\n", *shared_data);

    if (sem_post(lock) == -1) {
      exit(-1);
    }  
  }
  
  return 0;
}
