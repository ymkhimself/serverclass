#include "backup.h"

void copyfile(const char *src, const char *dest, FILE *stream) {
  int len;
  char buff[BUF_SIZE];
  FILE *in, *out;

  in  = fopen(src, "r+");
  out = fopen(dest, "w+");

  while (len = fread(buff, 1, sizeof(buff), in)) {
    fwrite(buff, 1, len, out);
  }

  fclose(in);
  fclose(out);

  debug(INFO_CP_FILE, dest);
  log(stream, INFO_CP_FILE, dest);
}

void removeHead(const char *str, const char *rep, char *mid) {
  int i = 0, j = 0;
  while (*(rep + i) != '\0') if (str[i] == rep[i]) i++; else break;
  while (*(str + i) != '\0') mid[j++] = str[i++];
  mid[j] = '\0';
}

void backupData(const char *pathname, const char *pathorg, FILE *stream) {
  DIR  *dp;
  struct dirent *dirp;
  struct stat st      = {0};
  char PATH_CUR[256]  = {0};
  char PATH_SRC[256]  = {0};
  char PATH_MID[256]  = {0};
  char PATH_DEST[256] = {0};

  if (NULL == (dp = opendir(pathname)) ) return;
  while ( NULL != ( dirp = readdir(dp))) {

    if (dirp->d_type == DT_DIR) {
      // 检测当前路径和父路径
      checkSp(dirp->d_name);
      // 生成原始目录的绝对路径
      snprintf(PATH_CUR, sizeof(PATH_CUR) - 1, "%s%s/", pathname, dirp->d_name);
      // 去掉文件名中的原始路径
      removeHead(PATH_CUR, pathorg, PATH_MID);
      // 生成目标目录的绝对路径
      snprintf(PATH_DEST, sizeof(PATH_DEST) - 1, "%s%s", ADDR_DEST, PATH_MID);

      // 检测文件夹是否存在
      if (-1 == stat(PATH_DEST, &st)) {

        // 创建文件夹
        if (mkdir(PATH_DEST, DEFAULT_MODE)) {
          error(EXIT_FAILURE, errno, ERR_CR_DIR);
        } else {
          debug(INFO_CR_DIR, PATH_DEST);
          log(stream, INFO_CR_DIR, PATH_DEST);
        }
      }

      backupData(PATH_CUR, pathorg, stream);
    } else {
      // 生成原始文件的绝对路径
      snprintf(PATH_SRC, sizeof(PATH_SRC) - 1, "%s%s", pathname, dirp->d_name);
      // 去掉文件名的原始路径，比如`/usr/local/src/`中的`/usr/local`
      removeHead(pathname, pathorg, PATH_MID);
      // 生成目的文件的绝对路径
      snprintf(PATH_DEST, sizeof(PATH_DEST) - 1, "%s%s%s", ADDR_DEST, PATH_MID, dirp->d_name);
      // 拷贝文件
      copyfile(PATH_SRC, PATH_DEST, stream);
    }
  }
  closedir(dp);
}

void saveList(FILE *stream, char *buf, size_t len) {
  FILE *list;
  list = fopen(LIST_DEST, "a+");
  fwrite(buf, 1, len, list);
  fclose(list);
}

void handler() {
  time_t t;
  size_t len;
  char *buf;
  FILE *stream,*list;

  time(&t);
  stream = open_memstream(&buf, &len);
  log(stream, INFO_BK_HEAD, ctime(&t));

  debug(INFO_START);
  backupData(ADDR_SRC, ADDR_SRC, stream);
  debug(INFO_FINISH);

  saveList(stream, buf, len);
  fclose(stream);
  free(buf);

  alarm(CYCLE_TIME);
}
