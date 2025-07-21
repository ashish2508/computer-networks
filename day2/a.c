/*Write a C program to swap the content of 2 variables entered through the 
command line using function and pointer.
*/
#include<stdio.h>


void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

 int main(int argc, char *argv[]){
  if (argc != 3) {
    printf("Usage: %s <num1> <num2>\n", argv[0]);
    return 1;
  }
  int a=atoi(argv[1]);
  int b=atoi(argv[2]);
  puts("Before swap:");
  printf("%d %d\n", a, b);
  puts("After swap:");
  swap(&a, &b);
  printf("%d %d\n", a, b);
  return 0;
}
