#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  int sockfd;
  char buffer[1024];
  struct sockaddr_in servaddr, cliaddr;
  socklen_t len = sizeof(cliaddr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket failed");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(8080);

  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(1);
  }

  while (1) {
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';

    if (strcmp(buffer, "time") == 0) {
      time_t t = time(NULL);
      char *timestr = ctime(&t);
      sendto(sockfd, timestr, strlen(timestr), 0, (struct sockaddr *)&cliaddr,
             len);
    }
  }

  close(sockfd);
  return 0;
}
