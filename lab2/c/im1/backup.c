#include "backup.h"

FILE *LIST;
char SRC_DIR[BUF_SIZE];

// 输入需要备份的目录
int main(int argc, char *argv[]) {
    if (argc != 2) {
        err(ERR_INPUT_DIR);
        exit(EXIT_FAILURE);
    }
    // 得到需要备份的目录
    strcpy(SRC_DIR, argv[1]);
    signal(SIGALRM, handler);
    alarm(CYCLE_TIME);
    for (int i = 0;; i++) {
        if (i % CYCLE_TIME == 0) {
            i = 0;
        }
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}
