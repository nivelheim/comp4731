#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <limits.h>

#define NUSED 10

int main(void) {
  int *shared_data;
  int rfd, i;
  int shared_size = sizeof(int) * NUSED;

  rfd = shm_open("/shared_obj", O_RDWR, 0777);
  if(rfd < 0) {
    exit(-1);
  }

  shared_data = mmap(NULL, shared_size, PROT_READ | PROT_WRITE, MAP_SHARED, rfd, 0);

  if (shared_data == MAP_FAILED) {
    exit(-1);
  }

  for (i = 1; i <= shared_data[0]; i++) {
    printf("Shared objet is: %d\t\n", shared_data[i]);
  }

  if (munmap(shared_data, shared_size) < 0) {
    exit(-1);
  }

  if (shm_unlink("/shared_obj") != 0) {
    exit(-1);
  }

  return 0;
}
