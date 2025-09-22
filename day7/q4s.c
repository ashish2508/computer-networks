#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 1024
#define PORT 5000
int main() {
  struct sockaddr_in address;
  int server_fd, client_fd;
  int addrlen = sizeof(address);
  char buffer[N] = {0};
  char *msg = "Hello from server";
  FILE *fp = fopen("data.txt", "r");
  fread(buffer, 1, N, fp);
  buffer[N] = '\0';
  fclose(fp);
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  printf("Server listening on port %d . .  .\n", PORT);

  while (1) {
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    puts("Client Connected");
    printf("buffer: %s\n", buffer);
    send(client_fd, buffer, strlen(buffer), 0);
    close(client_fd);
    puts("Client Disconnected");
  }
  close(server_fd);
  return 0;
}
