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

int ftruncate(int fd, off_t length);

int main(void) {
  int *shared_data;
  sem_t *lock;
  int fd;
  int shared_size;

  shared_size = sizeof(int);

  //Create shared memory
  fd = shm_open(MEM, O_CREAT | O_RDWR, 0777);
  if (fd < 0) {
    perror("shm_open");
    exit(-1);
  }

  if(ftruncate(fd, shared_size) == -1) {
    perror("ftruncate");
    exit(-1); 
  }
  
  shared_data = mmap(NULL, shared_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  if (shared_data == MAP_FAILED) {
    perror("mmap");
    exit(-1);
  }

  //Create shared semaphore
  lock = sem_open(SEM, O_CREAT, 0777, 1);
  
  if (lock == SEM_FAILED) {
    perror("sem_open");
    exit(-1);
  }

  munmap(shared_data, shared_size);

  return 0;
}
