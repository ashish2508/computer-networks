#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *server_ip = argv[1];
  int server_port = atoi(argv[2]);

  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in client_addr, server_addr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&client_addr, 0, sizeof(client_addr));
  memset(&server_addr, 0, sizeof(server_addr));

  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = htons(0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(server_port);

  if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  fd_set fds;
  int n;
  socklen_t len;

  printf("Client is running. Enter messages to send to server (IP: %s, Port: "
         "%d). Type 'exit' to quit.\n",
         server_ip, server_port);

  while (1) {
    FD_ZERO(&fds);
    FD_SET(0, &fds); // stdin
    FD_SET(sockfd, &fds);

    int maxfd = (sockfd > 0 ? sockfd : 0) + 1;
    select(maxfd, &fds, NULL, NULL, NULL);

    if (FD_ISSET(0, &fds)) {
      fgets(buffer, MAXLINE, stdin);
      buffer[strcspn(buffer, "\n")] = '\0';
      sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr,
             sizeof(server_addr));
      if (strcmp(buffer, "exit") == 0) {
        printf("Sent 'exit'. Exiting...\n");
        break;
      }
    }

    if (FD_ISSET(sockfd, &fds)) {
      len = sizeof(server_addr);
      n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&server_addr,
                   &len);
      buffer[n] = '\0';
      printf("Received from server: %s\n", buffer);
      if (strcmp(buffer, "exit") == 0) {
        printf("Received 'exit'. Exiting...\n");
        break;
      }
    }
  }

  close(sockfd);
  return 0;
}
