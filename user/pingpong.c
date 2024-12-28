#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
  int p[2];
  char buf[10];

  pipe(p);  // p[0]: 讀取端,  p[1]: 寫入端

  if (fork() == 0) {  // child
    read(p[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(p[1], buf, 1);
  } else {            // parent
    buf[0] = 'p';
    write(p[1], buf, 1);
    sleep(1);         // 讓 parent sleep 換 child 執行
    read(p[0], buf, 1);
    printf("%d: received pong\n", getpid());
  }

  exit(0);
}