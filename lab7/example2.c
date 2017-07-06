#include <unistd.h>
#include <fcntl.h>

int main(void) {
  int fd = open("xxx", O_CREAT | O_WRONLY | O_TRUNC, 0660);
  
  if (fd==-1) {
    perror("open");
    return 1
  }

  dup2(fd,1);
  write(1, "hello\n", 6);
  write(2, "world\n", 6);

  return 0;
}
