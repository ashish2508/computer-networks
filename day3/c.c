#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAXLINE 1024

int main() {
  WSADATA wsaData;
  SOCKET sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in server_addr, client_addr;
  int addr_len = sizeof(client_addr);
  int n;

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    printf("WSAStartup failed.\n");
    return 1;
  }

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == INVALID_SOCKET) {
    printf("Socket creation failed: %d\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      SOCKET_ERROR) {
    printf("Bind failed: %d\n", WSAGetLastError());
    closesocket(sockfd);
    WSACleanup();
    return 1;
  }

  printf("UDP server running at 127.0.0.1:%d...\n", PORT);

  while (1) {
    n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&client_addr,
                 &addr_len);
    if (n == SOCKET_ERROR) {
      int err = WSAGetLastError();
      if (err == WSAEWOULDBLOCK) {
        Sleep(100);
        continue;
      }
      printf("recvfrom failed: %d\n", err);
      break;
    }

    buffer[n] = '\0';
    printf("Received from client: %s\n", buffer);

    if (sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client_addr,
               addr_len) == SOCKET_ERROR) {
      printf("sendto failed: %d\n", WSAGetLastError());
      break;
    }
  }

  closesocket(sockfd);
  WSACleanup();
  return 0;
}
