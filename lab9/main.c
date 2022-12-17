#include "include/microhttpd.h"
#include <alloca.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PAGE "static/index.html"
#define FAVICON "static/favicon.ico"
#define CYCLE_TIME 5 // 5秒一刷

static enum MHD_Result ahc_echo(void *cls, struct MHD_Connection *connection,
                                const char *url, const char *method,
                                const char *version, const char *upload_data,
                                size_t *upload_data_size, void **ptr) {
  static int dummy;
  struct MHD_Response *response;
  int ret;

  if (0 != strcmp(method, "GET"))
    return MHD_NO; /* 非正常模式 */
  if (&dummy != *ptr) {
    /* 第一次只有 head 是合法的， 因此第一次不返回响应数据 */
    *ptr = &dummy;
    return MHD_YES;
  }
  if (0 != *upload_data_size)
    return MHD_NO; /*  get 模式不支持上传 */
  *ptr = NULL;     /* 清空内存指正 */

  if (0 == strcmp(url, "/")) {
    struct stat st;
    stat(PAGE, &st);
    int fd = open(PAGE, O_RDONLY);
    response = MHD_create_response_from_fd(st.st_size, fd); // 返回html

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
  } else if (0 == strcmp(url, "/favicon.ico")) { // 返回图标
    struct stat st;
    stat(FAVICON, &st);
    int fd = open(FAVICON, O_RDONLY);
    response = MHD_create_response_from_fd(st.st_size, fd);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
  } else {
    char process[20];
    strcpy(process, url + 1); // 获取url中的进程号
    int pid = atoi(process);
    int res;
    if (pid != 0) {
      res = kill(pid, SIGKILL);
    }
    char resp[200];
    if (res != 0) {
      if (res == EINVAL) {
        sprintf(resp, "An invalid signal was specified.\n");
      } else if (res == EPERM) {
        sprintf(resp,
                "{The process does not have permission to send the signal "
                "to any of the target processes.}");
      } else if (res == ESRCH) {
        sprintf(resp, "{The  pid or process group does not exist.");
      } else {
        sprintf(resp, "fail");
      }
      printf("%s\n", resp);
      response = MHD_create_response_from_buffer(strlen(resp), resp,
                                                 MHD_RESPMEM_PERSISTENT);
      MHD_add_response_header(response, "Content-Type",
                              "text/plain;charset=utf-8");
      ret = MHD_queue_response(connection, 500, response);
    } else {
      printf("kill %d\n", pid);
      sprintf(resp, "success");
      response = MHD_create_response_from_buffer(strlen(resp), resp,
                                                 MHD_RESPMEM_PERSISTENT);
      MHD_add_response_header(response, "Content-Type",
                              "text/plain;charset=utf-8");
      ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    }
    MHD_destroy_response(response);
  }
  return ret;
}

// 执行更新脚本
void refresh() { system("./test.sh"); }

int main(int argc, char **argv) {
  struct MHD_Daemon *d;

  if (argc != 2) {
    printf("%s PORT\n", argv[0]);
    return 1;
  }
  d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, atoi(argv[1]), NULL, NULL,
                       &ahc_echo, NULL, MHD_OPTION_END);
  if (d == NULL)
    return 1;
  // 开启定时任务
  signal(SIGALRM, refresh);
  alarm(CYCLE_TIME);
  (void)getc(stdin);
  MHD_stop_daemon(d);
  return 0;
}