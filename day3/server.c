#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(int argc, char const *argv[]) {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};
  char message[BUFFER_SIZE];
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
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
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) <
      0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  printf("Client connected. Start chatting (type 'bye' to exit).\n");
  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t valread = read(new_socket, buffer, BUFFER_SIZE - 1);
    if (valread <= 0) {
      printf("Client disconnected.\n");
      break;
    }
    printf("Client: %s\n", buffer);
    if (strcmp(buffer, "bye") == 0) {
      printf("Chat ended by client.\n");
      break;
    }
    printf("Server: ");
    fgets(message, BUFFER_SIZE, stdin);
    message[strcspn(message, "\n")] = 0; // Remove newline
    send(new_socket, message, strlen(message), 0);
    if (strcmp(message, "bye") == 0) {
      printf("Chat ended.\n");
      break;
    }
  }
  close(new_socket);
  close(server_fd);
  return 0;
}
