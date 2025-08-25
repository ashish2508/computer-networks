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

  fd_set fds;
  int n;
  socklen_t len;
  int client_connected = 0;
  printf("Server is running on port %d. Waiting for messages. Type 'exit' to "
         "quit.\n",
         server_port);

  while (1) {
    FD_ZERO(&fds);
    FD_SET(0, &fds); // stdin
    FD_SET(sockfd, &fds);

    int maxfd = (sockfd > 0 ? sockfd : 0) + 1;
    select(maxfd, &fds, NULL, NULL, NULL);

    if (FD_ISSET(0, &fds)) {
      if (!client_connected) {
        printf("No client connected yet. Cannot send.\n");
        continue;
      }
      fgets(buffer, MAXLINE, stdin);
      buffer[strcspn(buffer, "\n")] = '\0';
      sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr,
             sizeof(client_addr));
      if (strcmp(buffer, "exit") == 0) {
        printf("Sent 'exit'. Exiting...\n");
        break;
      }
    }
    int res = 0;
    if (FD_ISSET(sockfd, &fds)) {
      len = sizeof(client_addr);
      n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&client_addr,
                   &len);
      buffer[n] = '\0';
      printf("Received from client: %s\n", buffer);
      if (buffer[1] == '+') {
        int a = atoi(&buffer[0]);
        printf("%d\n", a);
        int b = atoi(&buffer[2]);
        printf("%d\n", b);
        res = a + b;
      }
      if (buffer[1] == '-') {

        int a = atoi(&buffer[0]);
        printf("%d\n", a);
        int b = atoi(&buffer[2]);
        printf("%d\n", b);
        res = a - b;
      }
      if (buffer[1] == '*') {
        int a = atoi(&buffer[0]);
        printf("%d\n", a);
        int b = atoi(&buffer[2]);
        printf("%d\n", b);
        res = a * b;
      }
      if (buffer[1] == '/') {
        int a = atoi(&buffer[0]);
        printf("%d\n", a);
        int b = atoi(&buffer[2]);
        printf("%d\n", b);
        res = a / b;
      }
    }

    sendto(sockfd, &res, sizeof(res), 0, (struct sockaddr *)&client_addr,
           sizeof(client_addr));
    client_connected = 1;
    if (strcmp(buffer, "exit") == 0) {
      printf("Received 'exit'. Exiting...\n");
      break;
    }
  }

  close(sockfd);
  return 0;
}
