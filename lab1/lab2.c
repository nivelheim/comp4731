#include <unistd.h>
#include <ctype.h>

#define LINESIZE 1024

int main() {
  char  line[LINESIZE];
  size_t  nbytes;

  while (1) {
    nbytes = read(0, line, LINESIZE);

    if (nbytes > 0) {
      for (char *c = line; *c; *c = toupper(*c), c++);
      write(1, line, nbytes);
    }

    else {
      return 1;
    }
  }

  return 0; 
}


