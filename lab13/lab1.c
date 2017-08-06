#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <grp.h>
#include <grp.h>

char* octetToChar(int mode) {
  const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
  
  static  char pms[11];
  char c;

  if (S_ISREG(mode))
    c = '-';
  else if (S_ISDIR(mode))
    c = 'd';
  else if (S_ISBLK(mode))
    c = 'b';
  else if (S_ISCHR(mode))
    c = 'c';
  else if (S_ISFIFO(mode))
    c = 'p';
  else if (S_ISLNK(mode))
    c = 'l';
  else {
    c = '?';
  }
  
  pms[0] = c;

  strcpy(&pms[1], rwx[(mode >> 6) & 7]);
  strcpy(&pms[4], rwx[(mode >> 3) & 7]);
  strcpy(&pms[7], rwx[(mode & 7)]);

  if (mode & S_ISUID)
    pms[3] = (mode & S_IXUSR) ? 's' : 'S';
  if (mode & S_ISGID)
    pms[6] = (mode & S_IXGRP) ? 's' : 'l';
  if (mode & S_ISVTX)
    pms[9] = (mode & S_IXOTH) ? 't' : 'T';
  pms[10] = '\0';
  
  return (pms);

}

int main(int argc, char *argv[]) {
  int fd;
  struct stat fst;
  struct passwd *pws;
  struct group *grp;

  for (int i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDWR)) == -1) {
      perror("open");
      exit(-1);
    }

    if ((fstat(fd, &fst)) == -1) {
      perror("fstat");
      exit(-1);
    }

    if ((pws = getpwuid(fst.st_uid)) == NULL) {
      perror("uid");
      exit(-1);
    }

    if ((grp = getgrgid(fst.st_gid)) == NULL) {
      perror("gid");
      exit(-1);
    }
      
    printf("name\t\t\t: %s\n", argv[i]);
    printf("device #\t\t: %ld\n", (long)fst.st_rdev);
    printf("i-num\t\t\t: %ld\n", (long)fst.st_ino);
    printf("# of links\t\t: %ld\n", (long)fst.st_nlink);
    printf("user id\t\t\t: %ld (%s)\n", (long)fst.st_uid, pws->pw_name);
    printf("grp id\t\t\t: %ld (%s)\n", (long)fst.st_gid, grp->gr_name);
    
    unsigned int md = (unsigned int)fst.st_mode;
    printf("mode\t\t\t: %o (%s)\n", md, octetToChar(md));
    printf("size\t\t\t: %lld\n", (long long)fst.st_size);
    printf("access time\t\t: %s", ctime(&fst.st_atime));
    printf("mod time\t\t: %s", ctime(&fst.st_mtime));
    printf("status change time\t: %s", ctime(&fst.st_ctime));
    printf("\n");
  }

  return 0;
}
