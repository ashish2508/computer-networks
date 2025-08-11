#include <stdio.h>
#include <string.h>

void hexToBinary(char *hex) {
  printf("Binary: ");
  for (int i = 0; hex[i] != '\0'; i++) {
    int value;
    if (hex[i] >= '0' && hex[i] <= '9') {
      value = hex[i] - '0';
    } else if (hex[i] >= 'A' && hex[i] <= 'F') {
      value = 10 + (hex[i] - 'A');
    } else if (hex[i] >= 'a' && hex[i] <= 'f') {
      value = 10 + (hex[i] - 'a');
    } else {
      printf("Invalid hex digit\n");
      return;
    }
    for (int j = 3; j >= 0; j--) {
      printf("%d", (value >> j) & 1);
    }
    printf(" ");
  }
  printf("\n");
}

int main() {
  char hex[256];
  printf("Enter a hexadecimal number: ");
  scanf("%s", hex);
  hexToBinary(hex);
  return 0;
}
