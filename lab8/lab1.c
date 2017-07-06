#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>

#define BUFLEN 1024

void execOne(char* input[]) {
  pid_t pid = fork();

  if (pid == 0) {
    execvp(input[0], input);
  }
  else{
    wait(0);
  }
}

void execTwo(int fd[], char* input[]) {
  pid_t pid;

  pid = fork();

  switch(pid) {
    case -1:
      exit(-1);
    case 0:
        if (fd[1] != 1 && dup2(fd[1], 1) == -1) {
          exit(0);
        }

        if (execvp(input[0], input) == -1) {
          exit(-1);
        }
  }
}

void execThree(int fd[], char* input[]) {
  pid_t pid;

  pid = fork();
  switch (pid) {
    case -1: 
      exit(-1);
    case 0:
      close(fd[1]);

      if (fd[0] != 0 && dup2(fd[0], 0) == -1) {
        close(fd[0]);
      }

      if (execvp(input[0], input) == -1) {
        exit(-1);
      }
  }
}

int main(void) {
  pid_t pid;
  size_t nread;
  int status;
  int fd[2];

  while (1) {
    
  char* input = NULL;

  char* args[BUFLEN] = {0};
  char* pipe1[BUFLEN] = {0};
  char* pipe2[BUFLEN] = {0};

  int i, j, k;
  char* tok = NULL;

  printf("> ");
  fflush(stdout);
  nread = getline(&input, &nread, stdin);

  input[nread-1] = '\0';

  tok = strtok(input, "|");

  i = 0;
  while(tok != NULL) {
    args[i] = tok;
    tok = strtok(NULL, "|");
    i++;
  }

  j = 0;
  tok = strtok(args[0], " ");
  while(tok != NULL) {
    pipe1[j] = tok;
    tok = strtok(NULL, " ");
    j++;
  }

  k = 0;
  tok = strtok(args[1], " ");
  while(tok != NULL) {
    pipe2[k] = tok;
    tok = strtok(NULL, " ");
    k++;
  }

  pipe(fd);

  if (*pipe1 != NULL && *pipe2 != NULL) {
    execTwo(fd, pipe1);
    execThree(fd, pipe2);
  }

  else {
    execOne(pipe1);
  }

  close(fd[0]);
  close(fd[1]);

  while ((pid = wait(&status)) != -1);

  }// end null

  return 0;
} 

