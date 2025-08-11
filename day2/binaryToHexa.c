#include <stdio.h>
#include <string.h>

void binaryToHex(char *binary) {
  int len = strlen(binary);
  int padding = (4 - len % 4) % 4;
  char padded[256] = {0};
  for (int i = 0; i < padding; i++) {
    padded[i] = '0';
  }
  strcat(padded, binary);

  printf("Hexadecimal: ");
  for (int i = 0; i < strlen(padded); i += 4) {
    int value = 0;
    for (int j = 0; j < 4; j++) {
      value = value * 2 + (padded[i + j] - '0');
    }
    if (value < 10) {
      printf("%d", value);
    } else {
      printf("%c", 'A' + (value - 10));
    }
  }
  printf("\n");
}

int main() {
  char binary[256];
  printf("Enter a binary number: ");
  scanf("%s", binary);
  binaryToHex(binary);
  return 0;
}
