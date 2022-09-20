#include "backup.h"

void handler() {
  debug(INFO_START);
  // 生成 shell 命令字符串
  snprintf(STR_CMD, sizeof(STR_CMD) - 1, INFO_CMD, LIST_DEST, ADDR_SRC, ADDR_DEST, LIST_DEST);
  // 调用系统执行shell 命令
  system(STR_CMD);
  debug(INFO_FINISH);

  alarm(CYCLE_TIME);
}
