#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define CYCLE_TIME 4
#define BUF_SIZE 256
#define ERR_INPUT_DIR "Please input the directory to backup\n"
#define DST_DIR "/home/ymk/server/lab2/c/backup"

#define log(...) printf(__VA_ARGS__)
#define err(...) fprintf(stderr, __VA_ARGS__)

extern void handler();
extern char SRC_DIR[BUF_SIZE];