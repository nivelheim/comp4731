#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define NUSED 10
#define MAXINPUT 1024

int ftruncate(int fd, off_t length);

int main(void) {
  char input[MAXINPUT];
  int *shared_data;
  int fd, i;
  int shared_size = sizeof(int) * NUSED;

  shm_unlink("/shared_obj");
  printf("%d\n", shared_size);

  fd = shm_open("/shared_obj", O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
  if (fd < 0) {
    perror("open error");
    exit(-1);
  }
  
  if (ftruncate(fd, shared_size) == -1) {
    perror("truncate error");
    exit(-1);
  }
  
  shared_data = mmap(NULL, shared_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_data == MAP_FAILED) {
    perror("mapping error");
    exit(-1);
  }

  for (i = 1; i < NUSED; i++) {
    printf("Enter an integer: ");
    if (fgets(input, MAXINPUT, stdin) == NULL) {
      printf("\n");
      shared_data[0] = i-1;
      break;
    }
    sscanf(input, "%d", &shared_data[i]);
    printf("%d\n", shared_data[i]); 
    shared_data[0] = i;  
  }

  printf("count: %d\n", shared_data[0]);
}
