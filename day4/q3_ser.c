#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int sockfd;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in servaddr, cliaddr;
  socklen_t len;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Bind failed");
    close(sockfd);
    exit(1);
  }

  printf("UDP Uppercase Server running on port %d...\n", PORT);

  while (1) {
    len = sizeof(cliaddr);

    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    for (int i = 0; buffer[i]; i++) {
      buffer[i] = toupper((unsigned char)buffer[i]);
    }

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, len);
  }

  close(sockfd);
  return 0;
}
