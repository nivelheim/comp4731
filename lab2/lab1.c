#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define LINESIZE 1024

int main(int argc, char *argv[]) {
  char  line[LINESIZE];
  size_t  nbytes;
  int fileopen, filesave;
  
  if (argc != 3) {
    perror("valid arguments must be provided");
    return 1;
  }

  if ((fileopen = open(argv[1], O_RDONLY)) == -1) {
    perror("Error opening source file");
    return 1;
  }

  if ((filesave = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC ,0660)) == -1) {
    perror("File already exists");
    return 1;
  } 

  while (1) {
    nbytes = read(fileopen, line, LINESIZE);
    
    if (nbytes > 0) { 
      write(filesave, line, nbytes);
    }
    
    else {
      return 1;
    }
  }

  close(fileopen);
  close(filesave);
  
  return 0; 
}
