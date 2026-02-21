#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "storage.h"
#include "ui.h"

#define INITIAL_CAPACITY 2
#define FILENAME "students.txt"
#define MENU_MIN 0
#define MENU_MAX 4

int main() {

    clearScreen();

    animateAction("Initializing system", 9, 200000);

    // initial memory allocation
    int size = 0, capacity = INITIAL_CAPACITY;
    Student *students = malloc(capacity * sizeof(Student));
    if (!students) {
        printf(RED "Memory allocation failed.\n" RESET);
        return 1;
    }

    loadStudents(&students, &size, &capacity, FILENAME);

    int choice;
    do {
        clearScreen();

        printf(" --- Student Management System ---\n");
        printf("Press 1: Add Student\n");
        printf("Press 2: View All Students\n");
        printf("Press 3: Save to File\n");
        printf("Press 4: Delete from File\n");
        printf("Press 0: Save and Exit / Exit\n\n");
        
        // Input choice with validation
        do {
            printf("Enter your choice: ");
            if (scanf("%d", &choice) != 1) {
                while (getchar() != '\n'); // clear invalid input
                choice = -1; // force loop
            }
            else { // only clear the buffer if scanf succeeded
                getchar();
            }

            if (choice < MENU_MIN || choice > MENU_MAX) {
                printf(RED "Please enter a valid choice from the menu.\n" RESET);
            }
        } while (choice < MENU_MIN || choice > MENU_MAX);

        printf("\n");

        switch (choice) {
            case 1:
                ensureCapacity(&students, &size, &capacity);
                inputStudent(students, size, &size);
                size++;
                animateAction("Adding student", 5, 200000);
                printf(GREEN "Student added successfully!\n" RESET);
                pressEnter();
                break;

            case 2:
                if (size == 0)
                {
                    printf(YELLOW "No students.\n" RESET);
                    pressEnter();
                    break;
                }

                sortStudents(students, size);
                animateAction("Loading student data", 7, 150000);
                printf("--- Student List ---\n\n");
                for (int i = 0; i < size; i++)
                {
                    printStudent(&students[i]);
                }
                pressEnter();
                break;

            case 3:
                saveStudents(students, size, FILENAME);
                printf(GREEN "Data saved successfully.\n" RESET);
                pressEnter();
                break;

            case 4: 
                deleteStudent(students, &size);
                pressEnter();
                break;

            case 0:
                printf(YELLOW "Do you want to save changes before exiting? (y/n): " RESET);
                char confirm;
                scanf(" %c", &confirm);
                if (toupper(confirm) == 'Y')
                {
                    saveStudents(students, size, FILENAME);
                    printf(GREEN "Data saved successfully.\n" RESET);
                }
                animateAction("Exiting system", 7, 200000);
                printf(GREEN "Successfully ended!\n\n" RESET);
                break;

            default :
                printf(RED "Invalid choice!\n" RESET);
                pressEnter();
        }
    } while (choice != 0);

    free(students); // free the memory
    return 0;
}