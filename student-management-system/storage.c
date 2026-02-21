#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"
#include "model.h"
#include "ui.h"

// Write into file
void saveStudents(Student *arr, int size, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf(RED "Failed to open file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(fp, "%d;%s;%d;%d;%d\n", arr[i].id, arr[i].name,
                arr[i].marks[0], arr[i].marks[1], arr[i].marks[2]);
    }

    fclose(fp);
}

// Read from file
void loadStudents(Student **arr, int *size, int *capacity, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return; // no file yet

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        ensureCapacity(arr, size, capacity); // ensure there is space before adding a student back to program

        char *token = strtok(line, ";"); // split lines into parts separated by semicolon
        (*arr)[*size].id = atoi(token);

        token = strtok(NULL, ";"); // NULL -> continue from where last token ended
        strcpy((*arr)[*size].name, token);

        for (int j = 0; j < NUM_SUBJECTS; j++) {
            token = strtok(NULL, ";");
            (*arr)[*size].marks[j] = atoi(token);
        }

        calculateResults(*arr, *size);
        (*size)++;
    }

    fclose(fp);
}