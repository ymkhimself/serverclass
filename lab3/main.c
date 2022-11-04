#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * 1. 时间，什么进程，错误信息. error,failed,Invalid,failure
 *
 */

#define DST "/home/ymk/error.log"
#define NUM_MSG 4

int num = 0;

char *msg[] = {"error", "Failed", "Invalid", "failure"};

// 按行处理日志
void handleLine(FILE *fout, char *line) {
  for (int i = 0; i < NUM_MSG; i++) {
    if (strstr(line, msg[i])) {  // 判断是否包含关键字
      // 处理日志，只显示时间，进程，错误信息
      char buf[255] = {0};
      int num_space = 0;
      int i = 0, j = 0;
      while (i < strlen(line)) {
        if (line[i] == ' ') {
          num_space++;
        }
        if (num_space != 3) {
          buf[j++] = line[i++];
        } else {
          i++;
        }
      }
      fprintf(fout, "%s", buf);
      num++;
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *fin = NULL;
  FILE *fout = NULL;
  char buffer[500] = {0};
  int ret = 0;

  if (argc != 2) {
    printf("Please provide the filename as argument to read\n");
    exit(EXIT_FAILURE);
  }

  fin = fopen(argv[1], "r");
  fout = fopen(DST, "w");
  if (fin == NULL) {
    printf("Error in opening the file error=%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (fout == NULL) {
    printf("Error in opening the file error=%s  %s\n", strerror(errno),DST);
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, sizeof(buffer), fin) != NULL) {
    handleLine(fout, buffer);
    memset(buffer, 0, sizeof(buffer));
  }
  printf("%d\n", num);
  fclose(fin);
  fclose(fout);

  return EXIT_SUCCESS;
}
