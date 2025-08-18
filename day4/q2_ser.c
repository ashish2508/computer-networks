#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <server_port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int server_port = atoi(argv[1]);

  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in server_addr, client_addr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(server_port);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(client_addr);
  int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&client_addr,
                   &len);
  buffer[n] = '\0';

  printf("Received from client: %s\n", buffer);

  // optional: reply back
  char reply[] = "Message received";
  sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *)&client_addr, len);

  printf("Exiting server...\n");
  close(sockfd);
  return 0;
}
