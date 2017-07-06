#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  pid_t pid;

  pid = fork();

  if (pid == -1) {
    perror("Error occured");
    return 1;
  }  
  
  if (pid == 0) {
    if (argc > 1) {
      execlp("ls", "ls", "-l", argv[1], (char*)0);
    }

    else if(argc == 1) {
      execlp("ls", "ls", "-l", (char*)0);
    }
  }

  else {
    wait(0);
  }
}
