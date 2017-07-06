#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
  int fd;
  char *fileMem;

  //  struct stat filestat;

  if ((fd = open(argv[1], O_RDWR)) < 0) {
    exit(-1);
  }

  int length = atoi(argv[2]);

  fileMem = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (fileMem == MAP_FAILED) {
    exit(-1);
  }

  for (int i=0; i<length; i++) {
    fileMem[i] = toupper(fileMem[i]);
  }

  if (munmap(fileMem, length) < 0) {
    exit(-1);
  }

  printf("%d\n", argc);
  return 0;
}
