#include <unistd.h>

int main(void) {
  dup2(2,1); //writing to 1 will be written on 2
  write(1, "hello\n", 6);
  write(2, "world\n", 6);
  return 0;
}

