#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int sockfd;
  char buffer[1024];
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080);
  servaddr.sin_addr.s_addr = INADDR_ANY; // assumes server on localhost

  while (1) {
    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr,
           sizeof(servaddr));

    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    if (n < 0)
      continue;
    buffer[n] = '\0';

    printf("Server reply: %s\n", buffer);
  }

  close(sockfd);
  return 0;
}
