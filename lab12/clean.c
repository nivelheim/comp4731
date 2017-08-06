#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <limits.h>

#define MEM "/sharedMem"
#define SEM "/sharedSem"

int main(void) {
  if (sem_unlink(SEM) == -1) {
    perror("sem_unlink");
    exit(-1);
  }

  if (shm_unlink(MEM) != 0) {
    perror("shm_unlink");
    exit(-1);
  }

  fprintf(stdout, "Cleanup Successfully Done\n");

  return 0;
}
