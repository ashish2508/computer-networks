/*
Write a C program to check whether the Host machine is in Little Endian or 
Big Endian. Enter a number, print the content of each byte location and 
Convert the Endianness of the same i.e. Little to Big Endian and vice-versa.
*/

#include <stdio.h>

int main() {
    unsigned int num = 1;
    unsigned char bytes[4];

    for (int i = 0; i < 4; i++) {
        bytes[i] = ((unsigned char*)&num)[i];
    }

    if (bytes[0] == 1) {
        printf("Little Endian\n");
    } else {
        printf("Big Endian\n");
    }

    printf("Original bytes: ");
    for (int i = 0; i < 4; i++) {
        printf("%02x ", bytes[i]);
    }
    printf("\n");

    unsigned char temp;
    temp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = temp;

    temp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = temp;

    printf("Converted bytes: ");
    for (int i = 0; i < 4; i++) {
        printf("%02x ", bytes[i]);
    }
    printf("\n");

    return 0;
}
