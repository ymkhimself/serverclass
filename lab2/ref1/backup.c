#include "backup.h"

char ADDR_SRC[BUF_SIZE];

int main(int argc, char *argv[]) {
  if (2 != argc) {
    debug(INFO_INPUT_DIR);
    exit(EXIT_FAILURE);
  }

  // 将原始路径保存为全局变量
  strcpy(ADDR_SRC, argv[1]);
  // 注册信号量函数
  signal(SIGALRM, handler);
  // 定时发送信号
  alarm(CYCLE_TIME);

  for (int i = 0;;) {
    if (i % CYCLE_TIME == 0) i = 0;
    debug(INFO_TIME, ++i);
    sleep(1);
  }

  exit(EXIT_SUCCESS);
}
