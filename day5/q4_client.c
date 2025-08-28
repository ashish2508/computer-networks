#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int sockfd;
  char buffer[1024];
  struct sockaddr_in servaddr;
  socklen_t len = sizeof(servaddr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket failed");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  while (1) {
    char msg[100];
    printf("Enter request (time/exit): ");
    scanf("%s", msg);

    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&servaddr, len);

    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';

    printf("Server: %s\n", buffer);

    if (strcmp(msg, "exit") == 0)
      break;
  }

  close(sockfd);
  return 0;
}
