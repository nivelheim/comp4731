#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

void readDir(const char *path, int lvl) {
  DIR *dirStream;
  struct dirent *dirEntry;
  struct stat dirStat;
  int spaces = lvl*2;

  lstat(path, &dirStat);

  if (S_ISLNK(dirStat.st_mode)) {
    return;
  }

  if ((dirStream = opendir(path)) == NULL) {
    return;
  }

  if ((chdir(path)) == -1) {
    return;
  }

  fprintf(stderr, "%*s%s/\n", spaces, "", path);
  spaces = (lvl+1) * 2;

  while ((dirEntry = readdir(dirStream)) != NULL) {
    lstat(dirEntry->d_name, &dirStat);

    if (S_ISDIR(dirStat.st_mode)) {
      if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      
      readDir(dirEntry->d_name, lvl+1);
    }

    else {
      fprintf(stderr, "%*s%s\n", spaces, "", dirEntry->d_name);
    }
  }

  chdir("..");
  //rewinddir(dirStream);
  closedir(dirStream);
}

int main(int argc, char *argv[]) {
  while (--argc > 0) {
    readDir(*++argv, 0);
  }

  return 0;
}
