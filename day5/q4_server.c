#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int sockfd;
  char buffer[1024];
  char reply[2048];
  struct sockaddr_in servaddr, cliaddr;
  socklen_t len;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
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

  printf("UDP server running on port 8080...\n");

  while (1) {
    len = sizeof(cliaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&cliaddr, &len);
    if (n < 0)
      continue;

    buffer[n] = '\0';

    printf("Received from client %s:%d -> %s\n", inet_ntoa(cliaddr.sin_addr),
           ntohs(cliaddr.sin_port), buffer);

    snprintf(reply, sizeof(reply),
             "Hello Client (%s:%d), I got your message: %s",
             inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);

    sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *)&cliaddr, len);
  }

  close(sockfd);
  return 0;
}
