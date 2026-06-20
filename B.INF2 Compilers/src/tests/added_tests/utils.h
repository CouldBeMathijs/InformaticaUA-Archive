#ifndef UTILS_H
#define UTILS_H

#define PI_APPROX 3
#define SUCCESS_CODE 0

struct StudentInfo {
    int id;
    char grade;
};
typedef struct StudentInfo Student;

// Een simpele functie declaratie om #include te testen
void print_header() {
    printf("--- Demo ---\n");
}

#endif
