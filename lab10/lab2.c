#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(void) {
  int *fileMap;
  int status;

  if ((fileMap = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
    perror("Mapping error");
    exit(-1);
  }

  *fileMap = 0;
  status = fork();

  if (status == -1) {
    perror("fork error");
    exit(-1);
  }

  else if (status == 0) {
    printf("file data is: %d\n", *fileMap);
    *fileMap = 5;

    if (munmap(fileMap, sizeof(int)) == -1) {
      perror("munmap error");
      exit(-1);
    }
  }

  else {
    wait(&status);
    printf("file data is :%d\n", *fileMap);

    if (munmap(fileMap, sizeof(int)) == -1) {
      perror("munmap error");
      exit(-1);
    }
  }
  
  return 0;

}


