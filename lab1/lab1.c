#include <unistd.h>

#define LINESIZE 1024

int main() {
  char  line[LINESIZE];  
  size_t  nbytes;

  while (1) {
    nbytes = read(0, line, LINESIZE);
   
    if (nbytes > 0) {
      write(1, line, nbytes);
    }
    
    else {
      return 1;
    } 
    
  }
  
  return 0; 
}
