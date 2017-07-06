#include <unistd.h>
#include <fcntl.h>

#define LINESIZE 1024

int main(int argc, char *argv[]) {
  char  line[LINESIZE];
  size_t  nbytes;
  int fileopen, filesave;
  
  if (argc != 3) {
    write(2, "valid arguments must be provided", 35);
    return 1;
  }

  if ((fileopen = open(argv[1], O_RDONLY)) == -1) {
    write(2, "Error opening source file", 30);
    return 1;
  }

  if ((filesave = creat(argv[2], 0660)) == -1) {
    write(2, "Error creating destination file", 35);
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
