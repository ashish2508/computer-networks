#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: %s <my_ip> <my_port> <peer_ip> <peer_port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *my_ip = argv[1];
  int my_port = atoi(argv[2]);
  char *peer_ip = argv[3];
  int peer_port = atoi(argv[4]);

  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in my_addr, peer_addr;

  // Create UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&my_addr, 0, sizeof(my_addr));
  memset(&peer_addr, 0, sizeof(peer_addr));

  // Self address
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = inet_addr(my_ip);
  my_addr.sin_port = htons(my_port);

  // Peer address
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_addr.s_addr = inet_addr(peer_ip);
  peer_addr.sin_port = htons(peer_port);

  // Bind the socket to own address
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  fd_set fds;
  int n;
  socklen_t len;
  int exit_flag = 0;

  printf("Enter messages to send. Type 'exit' to quit.\n");
  while (1) {
    FD_ZERO(&fds);
    FD_SET(0, &fds); // stdin
    FD_SET(sockfd, &fds);

    int maxfd = (sockfd > 0 ? sockfd : 0) + 1;
    select(maxfd, &fds, NULL, NULL, NULL);

    // If input available
    if (FD_ISSET(0, &fds)) {
      fgets(buffer, MAXLINE, stdin);
      buffer[strcspn(buffer, "\n")] = '\0';
      sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&peer_addr,
             sizeof(peer_addr));
      if (strcmp(buffer, "exit") == 0) {
        printf("Sent 'exit'. Exiting...\n");
        break;
      }
    }
    // If message received
    if (FD_ISSET(sockfd, &fds)) {
      len = sizeof(peer_addr);
      n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&peer_addr,
                   &len);
      buffer[n] = '\0';
      printf("Received: %s\n", buffer);
      if (strcmp(buffer, "exit") == 0) {
        printf("Received 'exit'. Exiting...\n");
        break;
      }
    }
  }
  close(sockfd);
  return 0;
}

/*
gcc socket.c -o a
 ./a 127.0.0.1 5000 127.0.0.1 5001

 */
