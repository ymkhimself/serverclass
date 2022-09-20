#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE          1024
#define CYCLE_TIME        2
#define INFO_START        "Start Backup....\n"
#define INFO_FINISH       "Finished....\n"
#define INFO_TIME         "%d seconds ...\n"
#define INFO_INPUT_DIR    "Please provide the directory to list out\n"
#define INFO_CMD          "date&>> %s && rsync -avz %s/. %s &>> %s"
#define ADDR_DEST         "/home/ymk/backup/"
#define LIST_DEST         "/usr/local/list"

#define debug(M,  ...)    fprintf(stderr, M, ##__VA_ARGS__)

extern void handler();
extern char STR_CMD[BUF_SIZE];
extern char ADDR_SRC[BUF_SIZE];
