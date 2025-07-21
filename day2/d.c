/*
 4. Write a C Program to enter a number and store the number across the 
following structure and print the content of each member of the structure. 
Then aggregate each member of the structure to form the original number and 
print the same.
struct pkt{
char ch1;
char ch2[2];
char ch3;
};
 */

#include <stdio.h>

struct pkt {
    char ch1;
    char ch2[2];
    char ch3;
};

int main() {
    int n;
    int digits[4] = {0, 0, 0, 0}; 
    printf("Enter a number (0 - 9999): ");
    scanf("%d", &n);

    if (n < 0 || n > 9999) {
        printf("Number out of range!\n");
        return 1;
    }

    
    int temp = n;
    for (int i = 3; i >= 0; i--) {
        digits[i] = temp % 10;
        temp /= 10;
    }

    struct pkt p;
    p.ch1 = (char)digits[0];
    p.ch2[0] = (char)digits[1];
    p.ch2[1] = (char)digits[2];
    p.ch3 = (char)digits[3];

    printf("p.ch1: %d\n", p.ch1);
    printf("p.ch2[0]: %d\n", p.ch2[0]);
    printf("p.ch2[1]: %d\n", p.ch2[1]);
    printf("p.ch3: %d\n", p.ch3);

    int originalFormed = p.ch1 * 1000 + p.ch2[0] * 100 + p.ch2[1] * 10 + p.ch3;
    printf("Reconstructed number: %d\n", originalFormed);

    return 0;
}

