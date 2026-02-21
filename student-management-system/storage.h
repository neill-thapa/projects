#ifndef STORAGE_H
#define STORAGE_H

#include "model.h"

void saveStudents(Student *arr, int size, const char *filename);
void loadStudents(Student **arr, int *size, int *capacity, const char *filename);

#endif