#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 *execl (vector; requires path)
 *execlp (path; path included)
 *execle (environment;)
 *(char*)0 indicates one of arguments
*/


int main(void) {
  pid_t pid;
  char *args[] = {"echo", "echo", "hello", "world", 0};

  pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  }

  if (pid == 0) { // child
    execl("/bin/cat", "cat", "fork1.c", (char *)0);
    execlp("echo", "echo", "hello", "world", (char*)0);
    execvp("echo", args);
  }
  else {
    wait(0);
  }
  
  return 0;
}

