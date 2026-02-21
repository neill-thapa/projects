#ifndef MODEL_H
#define MODEL_H

#define NUM_SUBJECTS 3

typedef struct {
    int id;
    char name[100];
    int marks[NUM_SUBJECTS];
    float average;
    char grade;
} Student;

void ensureCapacity(Student **arr, int *size, int *capacity);
int idExists(Student *arr, int size, int id);
void calculateResults(Student *arr, int index);
void sortStudents(Student *arr, int size);

#endif