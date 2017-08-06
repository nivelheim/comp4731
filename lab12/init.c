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

typedef struct {
  sem_t lock;
  unsigned long value;
} counter;

int ftruncate(int fd, off_t length);

int main(void) {
  counter *shared_data;
  int fd;

  fd = shm_open(SHMEM, O_CREAT | O_EXCL | O_RDWR, 0777);
  if (fd < 0) {
    exit(-1);
  }

  if(ftruncate(fd, sizeof(counter)) == -1) {
   exit(-1); 
  }
  
  shared_data = mmap(NULL, sizeof(counter), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_data ==MAP_FAILED) {
    exit(-1);
  }

  sem_init(&shared_data->lock, 1, 1);
  shared_data->value = 0;

  return 0;
  
}
