#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int sockfd;
  struct sockaddr_in servaddr;
  char buffer[BUFFER_SIZE];
  char message[BUFFER_SIZE];

  // Create socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

  while (1) {
    printf("Enter lowercase string (or 'exit' to quit): ");
    fgets(message, BUFFER_SIZE, stdin);

    message[strcspn(message, "\n")] = '\0';

    if (strcmp(message, "exit") == 0)
      break;

    sendto(sockfd, message, strlen(message), 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';

    printf("Uppercase from server: %s\n", buffer);
  }

  close(sockfd);
  return 0;
}
