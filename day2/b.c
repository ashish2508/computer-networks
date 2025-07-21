/*
 Write a C program to assign values to each member of the following structure. 
Pass the populated structure to a function Using call-by-value and another 
function using call-by-address and print the value of each member of the 
structure.
*/

#include <stdio.h>
#include <string.h>

struct dob {
    int day;
    int month;
    int year;
};

struct student_info {
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
};

void print_by_value(struct student_info s) {
    printf("Call-by-Value:\n");
    printf("Roll No: %d\n", s.roll_no);
    printf("Name: %s\n", s.name);
    printf("CGPA: %.2f\n", s.CGPA);
    printf("DOB: %d/%d/%d\n", s.age.day, s.age.month, s.age.year);
}

void print_by_address(struct student_info *s) {
    printf("Call-by-Address:\n");
    printf("Roll No: %d\n", s->roll_no);
    printf("Name: %s\n", s->name);
    printf("CGPA: %.2f\n", s->CGPA);
    printf("DOB: %d/%d/%d\n", s->age.day, s->age.month, s->age.year);
}

int main() {
    struct student_info student;
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
    printf("Enter Roll No: ");
    scanf("%d", &roll_no);
    printf("Enter Name: ");
    scanf(" %[^\n]", name);
    printf("Enter CGPA: ");
    scanf("%f", &CGPA);
    printf("Enter Date of Birth (DD MM YYYY): ");
    scanf("%d %d %d", &age.day, &age.month, &age.year);

    student.roll_no = roll_no;
    strcpy(student.name, name);
    student.CGPA = CGPA;
    student.age.day = age.day;
    student.age.month = age.month;
    student.age.year = age.year;

    print_by_value(student);
    print_by_address(&student);

    return 0;
}
