#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 100
#define PORT 5000

int main() {
  struct sockaddr_in serv_addr;
  int sock = 0;
  char s[MAXLINE] = {0};
  FILE *fp = fopen("f1.txt", "r");

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

  while (fgets(s, MAXLINE, fp) != NULL) {
    printf("%s", s);
    send(sock, s, strlen(s), 0);
  }

  fclose(fp);
  close(sock);
  return 0;
}
