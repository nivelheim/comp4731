#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFLEN 1024
void exec_noArg(char* input[]) {
  pid_t pid = fork();

  if (pid == 0) {
    execvp(input[0], input);
  }

  else{ 
    wait(0);
  }
}

void exec_oneArg(int opt, char* fileName, char* input[]) {
  int fd;
  //char* temp[2];
  
  pid_t pid;
   
  //temp[0] = input[0];
  //temp[1] = NULL;

  pid = fork();
  
  if (pid == 0) {
    if (opt == 0) {
      fd = open(fileName, O_RDONLY);
      dup2(fd, 0);
      close(fd);
      execvp(input[0], input);
    }

    else {
      fd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, 0666);
      dup2(fd, 1);
      close(fd);
      execvp(input[0], input);
    }
  }

  else {
    wait(0);
  } 
}

void exec_twoArg(char* fd0Name, char* fd1Name, char* input[]) {
  int fd0, fd1;
  //char* temp[2];

  pid_t pid;

  //temp[0] = input[0];  
  //temp[1] = NULL;
  
  fd0 = open(fd0Name, O_RDONLY);
  fd1 = open(fd1Name, O_CREAT | O_WRONLY | O_TRUNC, 0666);

  pid = fork();

  if (pid == 0) {
    dup2(fd0, 0);
    dup2(fd1, 1);

    close(fd0);
    close(fd1);

    execvp(input[0], input);
  }

  else{
    wait(0);
  }
}

int main(void) {
  const char delim[] = " ";
  
  while(1) {
  
  char* input = NULL;
  char* tok = NULL;
  
  size_t nread;

  char* args[BUFLEN] = {NULL};
  char* switches[BUFLEN] = {NULL};
  char* inArg = NULL;
  char* outArg = NULL;  
  
  int i = 0;
  int c = 0;
  int inPos = 0;
  int outPos = 0;
  
  
  printf(">");
  nread = getline(&input, &nread, stdin);
  
  input[nread-1] = '\0'; //replaces newline with null terminator

  //Tokenize input 
  tok = strtok(input, delim);
  while (tok != NULL) {
    args[i] = tok;
    tok = strtok(NULL, delim);
    i++;
  }
  
  //Loop through tokenized array and find last I/O position
  for (int j = 0; j<i; j++) {
    char* temp = args[j];
    if (temp[0] == '<') {
      inPos = j;
    }
    else if (temp[0] == '>') {
      outPos = j;
    }
    else {
      switches[c] = temp;
      c++;
    }
  }
  printf("%d, %d\n", inPos, outPos);

  //Only input redirection is provided
  if ((inPos != 0) && (outPos == 0)) {
    inArg = args[inPos];
    exec_oneArg(0, inArg+1, switches);
  }
  //Only output redirection is provided
  if ((inPos == 0) && (outPos != 0)) {
    outArg = args[outPos];
    exec_oneArg(1, outArg+1, switches);
  }
  //No redirection arguments provided
  if ((inPos == 0) && (outPos == 0)) {
    exec_noArg(switches);  
  }
  //Both redirection arguments provide
  if ((inPos != 0) && (outPos != 0)) {
    inArg = args[inPos];
    outArg = args[outPos];
    exec_twoArg(inArg+1, outArg+1, switches);
  }

  }//end while
  return 0;


}
