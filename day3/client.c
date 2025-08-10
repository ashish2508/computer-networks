#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(int argc, char const *argv[]) {
  int client_fd;
  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE] = {0};
  char message[BUFFER_SIZE];
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Socket creation error\n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address\n");
    return -1;
  }
  if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    printf("Connection failed\n");
    return -1;
  }
  printf("Connected to server. Start chatting (type 'bye' to exit).\n");
  while (1) {
    printf("Client: ");
    fgets(message, BUFFER_SIZE, stdin);
    message[strcspn(message, "\n")] = 0; // Remove newline
    send(client_fd, message, strlen(message), 0);
    if (strcmp(message, "bye") == 0) {
      printf("Chat ended.\n");
      break;
    }
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t valread = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (valread <= 0) {
      printf("Server disconnected.\n");
      break;
    }
    printf("Server: %s\n", buffer);
    if (strcmp(buffer, "bye") == 0) {
      printf("Chat ended by server.\n");
      break;
    }
  }
  close(client_fd);
  return 0;
}
