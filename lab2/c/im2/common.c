#include "backup.h"

void backupdata() {
    char *cmd[255] = {0};

    sprintf(cmd, "cp -a %s/* %s", SRC_DIR, DST_DIR);
    puts(cmd);
    int a = system(cmd);
    if (a != 0) {
        err("备份出错\n");
    }
}

void handler() {
    time_t t = time(0);
    char tmpBuf[255];
    strftime(tmpBuf, 255, "%Y-%m-%d %H:%M:%S", localtime(&t));
    log("%s Backup begin\n", tmpBuf);
    backupdata();
    alarm(CYCLE_TIME);
}