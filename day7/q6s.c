#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 9090

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int nums[2], sum;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  printf("Server listening on port %d...\n", PORT);

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("Accept failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  read(new_socket, nums, sizeof(nums));
  printf("Received numbers: %d and %d\n", nums[0], nums[1]);

  sum = nums[0] + nums[1];

  send(new_socket, &sum, sizeof(sum), 0);
  printf("Sent sum: %d\n", sum);

  close(new_socket);
  close(server_fd);
  return 0;
}
