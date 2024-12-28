#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int lfd){
  int num;
  read(lfd, &num, sizeof(int));
  if(num == 0) return;

  printf("prime %d\n", num);

  int rfd[2]; // p[0]: 讀取端,  p[1]: 寫入端
  pipe(rfd);

  if(fork() == 0) { // 傳到下一個 process
    close(lfd);
    close(rfd[1]);  // 關閉寫入端
    prime(rfd[0]);
    close(rfd[0]);
    exit(0);
  } else {          // 處理數字然後 pipe 過去
    close(rfd[0]);  // 關閉讀取端
    int n;
    while(read(lfd, &n, sizeof(int)) > 0){
      if(n % num != 0)
        write(rfd[1], &n, sizeof(int));
    }
    close(rfd[1]);
  }
  wait(0);
}

int main(int argc, char *argv[]){
  int p[2]; // p[0]: 讀取端,  p[1]: 寫入端
  pipe(p);

  if(fork() == 0) {
    close(p[1]);  // 關閉寫入端
    prime(p[0]);
  } else {
    close(p[0]);  // 關閉讀取端
    for(int i = 2; i <= 280; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}