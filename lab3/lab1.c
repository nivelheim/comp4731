#define _GNU_SOURCE
#define MAXARGS 1024

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(void) {
  char* input = NULL;
  char* tokens[MAXARGS];
  
  const char d[] = " ";

  size_t nread;

  pid_t pid;
  
  while(1) {
    int counter = 0;
    
    printf("> ");
    nread = getline(&input, &nread, stdin);
    printf("%zu\n", nread);

    input[nread-1] = '\0';

    char* tok = NULL;
    tok = strtok(input, d);

    while (tok != NULL && counter < MAXARGS) {
      tokens[counter] = tok;
      tok = strtok(NULL, d);
      counter++;
    }

    tokens[counter] = 0;

    for (int j =0; j<counter; j++) {
      printf("%s", tokens[j]);
    }

    pid = fork();

    if (pid == 0) {
      printf("test");
      execvp(tokens[0], tokens);
    }

    else{
      wait(0);
    }
  }
  

  return 0;
}
