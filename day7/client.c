#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
#define PORT 5000

int main() {
  struct sockaddr_in serv_addr;
  int sock = 0;
  char buffer[MAXLINE] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection Failed");
    exit(EXIT_FAILURE);
  }

  char *msg = "hi";
  send(sock, msg, strlen(msg), 0);
  printf("Message sent: %s\n", msg);

  int bytes_read = read(sock, buffer, MAXLINE);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    printf("Message from server: %s\n", buffer);
  }

  close(sock);
  return 0;
}
