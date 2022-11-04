#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SERV_TCP_PORT 23
#define MAX_SIZE 80

int main(int argc, char *argv[]) {
  time_t t;
  struct tm *lt;
  char string[MAX_SIZE];
  int port, len, sockfd, newsockfd, clilen;
  struct sockaddr_in cli_addr, serv_addr;

  if (argc == 2)
    /* 读取命令行参数的端口号 */
    sscanf(argv[1], "%d", &port);
  else
    port = SERV_TCP_PORT;

  /* 打开 TCP 套接口 */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("can't open stream socket");
    exit(1);
  }

  /* 绑定本地地址后, 客户端才能发送给服务器数据 */
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("can't bind local address");
    exit(1);
  }

  /* 监听端口 */
  listen(sockfd, 5);

  for (;;) {
    /* 等待客户端连接 */
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) { // 连接失败,报错
      perror("can't bind local address");
    }
    printf("连接建立\n");
    /* 读取客户端的消息 */
    while (1) {
      len = read(newsockfd, string, MAX_SIZE);
      string[len] = 0;
      if (strcmp(string, "s") == 0) {
        // 读取并打印字符串
        len = read(newsockfd, string, MAX_SIZE);
        string[len] = 0;
        printf("%s\n", string);
      } else if (strcmp(string, "t") == 0) {
        // 打印时间
        len = read(newsockfd, &t, sizeof(t));
        lt = localtime(&t);
        printf("%d/%d/%d %d:%d:%d\n", lt->tm_year + 1900, lt->tm_mon,
               lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
      } else if (strcmp(string, "q") == 0) {
        // 释放与客户端的连接
        printf("quit\n");
        close(newsockfd);
        break;
      }
    }
  }
}
