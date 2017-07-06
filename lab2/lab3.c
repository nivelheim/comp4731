#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  pid_t pid;
  int i;
  char* input[argc+3];

  input[0] = "ls";
  input[1] = "-l";
  input[2] = "--";
  input[argc+2] = 0;

  pid = fork();

  if (pid == -1) {
    perror("Error occured");
    return 1;
  }  
  
  if (pid == 0) {
    if (argc ==  1) {
      write(1, "single", 10);
      execlp("ls", "ls", (char*)0);
    }

    else {
      write(1, "multi", 10);
      for (i=3; i<(argc+2); i++) {
        input[i] = argv[i-2];
      }
      execvp("ls", input);
    }
  }

  else {
    wait(0);
  }

  return 0;
}
