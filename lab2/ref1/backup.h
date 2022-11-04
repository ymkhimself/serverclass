#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 256
#define CYCLE_TIME 6
#define DEFAULT_MODE S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
#define INFO_START "Start Backup....\n"
#define INFO_FINISH "Finished....\n"
#define INFO_TIME "%d seconds ...\n"
#define INFO_CP_FILE "copy file: %s\n"
#define INFO_CR_DIR "create folder: %s\n"
#define INFO_BK_HEAD "\n backup time: %s"
#define INFO_INPUT_DIR "Please provide the directory to list out\n"
#define ERR_CR_DIR "Failed to create directory!\n"
#define ADDR_DEST "/home/ymk/backup/"
#define LIST_DEST "/usr/local/list"

#define log(S, ...) fprintf(S, ##__VA_ARGS__)
#define debug(M, ...) fprintf(stderr, M, ##__VA_ARGS__)
#define checkSp(M) \
  if ((strcmp(M, ".") == 0) || (strcmp(M, "..") == 0)) continue;

extern void handler();
extern char ADDR_SRC[BUF_SIZE];
