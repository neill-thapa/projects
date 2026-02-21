#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "ui.h"

// Memory management
void ensureCapacity(Student **arr, int *size, int *capacity) {
    if (*size >= *capacity) {
        *capacity *= 2;
        Student *temp = realloc(*arr, (*capacity) * sizeof(Student));
        if (!temp) {
            printf(RED "Memory allocation failed.\n" RESET);
            exit(1); // critical failure
        }
        *arr = temp;
    }
}

// Check for id
int idExists(Student *arr, int size, int id) {
    for (int i = 0; i < size; i++) {
        if (arr[i].id == id) {
            return 1;
        }
    }
    return 0;
}

// Logic function
void calculateResults(Student *arr, int index) {
    int sum = 0;
    for (int j = 0; j < NUM_SUBJECTS; j++) {
        sum += arr[index].marks[j];
    }
    arr[index].average = sum / (float)NUM_SUBJECTS;

    if (arr[index].average >= 90) {
        arr[index].grade = 'A';
    }
    else if (arr[index].average >= 75) {
        arr[index].grade = 'B';
    }
    else if (arr[index].average >= 60) {
        arr[index].grade = 'C';
    }
    else if (arr[index].average >= 50) {
        arr[index].grade = 'D';
    }
    else {
        arr[index].grade = 'F';
    }
}

// Selection sort
void sortStudents(Student *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        int maxIndex = i;

        for (int j = i + 1; j < size; j++) {
            // Primary: higher average
            if (arr[j].average > arr[maxIndex].average) {
                maxIndex = j;
            }
            // Secondary: same average, sort by name alphabetically
            else if (arr[j].average == arr[maxIndex].average && 
                strcmp(arr[j].name, arr[maxIndex].name) < 0) {
                    maxIndex = j;
            }
        }

        if (maxIndex != i) {
            Student temp = arr[i];
            arr[i] = arr[maxIndex];
            arr[maxIndex] = temp;
        }
    }
}