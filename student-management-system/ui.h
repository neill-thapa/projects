#ifndef UI_H
#define UI_H

#include "model.h"

// Color codes
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

void inputStudent(Student *arr, int index, int *size);
void deleteStudent(Student *arr, int *size);
void printStudent(const Student *s);
void clearScreen();
void pressEnter();
void animateAction(const char *text, int dots, int delayUs);

#endif