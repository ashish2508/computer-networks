#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024
void to_uppercase(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper(str[i]);
  }
}
int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE];
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 1) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }
  printf("Server listening on port %d...\n", PORT);
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("Accept failed");
    exit(EXIT_FAILURE);
  }
  printf("Client connected.\n");
  int valread = read(new_socket, buffer, BUFFER_SIZE);
  buffer[valread] = '\0';
  printf("Received from client: %s\n", buffer);
  to_uppercase(buffer);
  send(new_socket, buffer, strlen(buffer), 0);
  printf("Modified string sent: %s\n", buffer);
  close(new_socket);
  close(server_fd);
  return 0;
}
