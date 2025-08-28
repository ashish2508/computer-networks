#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int sockfd;
  char buffer[1024];
  struct sockaddr_in servaddr, cliaddr;
  socklen_t len = sizeof(cliaddr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket failed");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(8080);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(1);
  }

  printf("UDP Math Server running on port 8080...\n");

  while (1) {
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';

    int a, b;
    char op;
    double result;
    char response[1024];

    if (sscanf(buffer, "%d %d %c", &a, &b, &op) == 3) {
      switch (op) {
      case '+':
        result = a + b;
        break;
      case '-':
        result = a - b;
        break;
      case '*':
        result = a * b;
        break;
      case '/':
        if (b == 0) {
          snprintf(response, sizeof(response), "Error: Division by zero");
          sendto(sockfd, response, strlen(response), 0,
                 (struct sockaddr *)&cliaddr, len);
          continue;
        }
        result = (double)a / b;
        break;
      default:
        snprintf(response, sizeof(response), "Error: Invalid operator");
        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&cliaddr, len);
        continue;
      }
      snprintf(response, sizeof(response), "Result = %.2f", result);
    } else {
      snprintf(response, sizeof(response),
               "Error: Invalid input format (use: num1 num2 op)");
    }
    printf("Received: %s, Sending: %s\n", buffer, response);
    sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&cliaddr,
           len);
  }

  close(sockfd);
  return 0;
}
