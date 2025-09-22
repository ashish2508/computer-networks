#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 9090
int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  int nums[2], sum;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
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

  printf("Enter first number: ");
  scanf("%d", &nums[0]);
  printf("Enter second number: ");
  scanf("%d", &nums[1]);

  send(sock, nums, sizeof(nums), 0);

  read(sock, &sum, sizeof(sum));
  printf("Sum received from server: %d\n", sum);

  close(sock);
  return 0;
}
