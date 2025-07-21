/*
 Write a C program to extract each byte from a given number and store them in 
separate character variables and print the content of those variables.
*/

#include <stdio.h>

int main() {
  unsigned int n;
  printf("Enter the number (e.g., 0x12345678):");
  scanf("%d", &n); 
  unsigned char *byte_ptr = (unsigned char *)&n; 
  puts("Extracted Bytes (order depends on system's endianness):\n");
  for (int i = 0; i < sizeof(unsigned int); i++) {
    printf("Byte %d: 0x%02X\n", i, byte_ptr[i]);
  }
  return 0;
}
