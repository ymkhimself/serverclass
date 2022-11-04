#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
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
  int sockfd;
  struct sockaddr_in serv_addr;
  char *serv_host = "localhost";
  struct hostent *host_ptr;
  char option[MAX_SIZE];
  char string[MAX_SIZE];
  int port;
  int buff_size = 0;
  time_t t;
  struct tm *lt;

  if (argc >= 2)
    /* 取 host 变量 */
    serv_host = argv[1];
  if (argc == 3)
    /* 取 port 变量 */
    sscanf(argv[2], "%d", &port);
  else
    port = SERV_TCP_PORT;

  /* 取 host 的主机地址 */
  if ((host_ptr = gethostbyname(serv_host)) == NULL) {
    perror("gethostbyname error");
    exit(1);
  }

  if (host_ptr->h_addrtype != AF_INET) {
    perror("unknown address type");
    exit(1);
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr =
      ((struct in_addr *)host_ptr->h_addr_list[0])->s_addr;
  serv_addr.sin_port = htons(port);

  /* 打开TCP套接口 */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("can't open stream socket");
    exit(1);
  }

  /* 连接远程服务器 */
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("can't connect to server");
    exit(1);
  }

  /* 发送数据到服务器 */
  while (1) {
    printf("s.send strings\nt.send time\nq.quit\n");
    scanf("%s", option);
    write(sockfd, option, sizeof(option));
    if (strcmp(option, "s") == 0) {
      // 输入字符串并发送
      scanf("%s", string);
      write(sockfd, string, sizeof(string));
    } else if (strcmp(option, "t") == 0) {
      time(&t); // 获取Unix时间戳。
      write(sockfd, &t, sizeof(t));
    } else if (strcmp(option, "q") == 0) {
      close(sockfd);
      break;
    }
  }
}