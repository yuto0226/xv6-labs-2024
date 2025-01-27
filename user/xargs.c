#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h" // MAXARG
#include "user/user.h"

void xargs(int argc, char *argv[])
{
  int pid, status;
  if ((pid=fork()) == 0) {
      exec(argv[0], argv);
      exit(1);
  }
  wait(&status);
  return;
}

int main(int argc, char *argv[])
{
  if(argc < 2 || argc - 1 > MAXARG) {
    fprintf(2,"[-] Usage: xargs <program> [params]\n");
    exit(1);
  }

  int xargi, bufi = 0;
  char c, buffer[256], *xargv[MAXARG];
  memset(buffer, 0, 256);

  for (xargi = 1; xargi < argc; xargi++)
        xargv[xargi - 1] = argv[xargi];

  while(read(0, &c, 1))
  {
    if(c == '\n') {
      if(bufi == 0) continue;
      buffer[bufi] = '\0';
      xargv[argc - 1] = buffer;
      xargs(argc + 1, xargv);
      bufi = 0;
      continue;
    }

    buffer[bufi++] = c;
  }

  if(bufi != 0)
  {
    buffer[bufi] = '\0';
    xargv[argc - 1] = buffer;
    xargs(argc + 1, xargv);
  }

  exit(0);
}