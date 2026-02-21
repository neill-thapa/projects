#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "ui.h"
#include "model.h"

// Input details
void inputStudent(Student *arr, int index, int *size) {
    char subjects[NUM_SUBJECTS][20] = {"Physics", "Chemistry", "Mathematics"};

    // input ID
    int id;
    do {
        printf("Enter student ID: ");
        scanf("%d", &id);
        getchar(); // clear newline
        if (idExists(arr, *size, id)) {
            printf(RED "ID already exists. Enter a unique ID.\n" RESET);
        }
    } while (idExists(arr, *size, id));
    arr[index].id = id;

    // input Name
    do {
        printf("Enter student Name: ");
        fgets(arr[index].name, sizeof(arr[index].name), stdin);
        arr[index].name[strcspn(arr[index].name, "\n")] = '\0'; // remove newline from fgets
        if (strlen(arr[index].name) == 0) {
            printf(RED "Name cannot be empty.\n" RESET);
        }
    } while (strlen(arr[index].name) == 0);

    // input Marks
    for (int j = 0; j < NUM_SUBJECTS; j++) {
        int marks;
        do {
            printf("Enter marks for %s (0-100): ", subjects[j]);
            if (scanf("%d", &marks) != 1) {
                while (getchar() != '\n');
                marks = -1;
            }
            else {
                getchar();
            }
            if (marks < 0 || marks > 100){
                printf(RED "Invalid marks! Must be between 0 and 100.\n" RESET);
            }
        } while (marks < 0 || marks > 100);
        arr[index].marks[j] = marks;
    }

    calculateResults(arr, index);
}

// Delete item
void deleteStudent(Student *arr, int *size) {
    if (*size == 0) {
        printf(YELLOW "\nNo students available to delete.\n" RESET);
        return;
    }

    int id;
    printf("Enter the ID of the student to delete: ");
    scanf("%d", &id);
    getchar();  // clear newline

    int index = -1; // assume not found

    // Search for student by ID
    for (int i = 0; i < *size; i++) {
        if (arr[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(RED "\nStudent with ID %d not found.\n" RESET, id);
        return;
    }

    // Show confirmation
    printf("\nFound student:\n");
    printStudent(&arr[index]);

    char confirm;
    printf(YELLOW "Are you sure you want to delete this student? (y/n): " RESET);
    scanf(" %c", &confirm);

    if (toupper(confirm) != 'Y') {
        printf(YELLOW "\nDeletion cancelled.\n" RESET);
        fflush(stdout);
        return;
    }

    // Shift elements left
    for (int i = index; i < *size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*size)--;
    printf(GREEN "Student deleted successfully.\n" RESET);
    fflush(stdout);
}

// Utility
void printStudent(const Student *s) {
    char subjects[NUM_SUBJECTS][20] = {"Physics", "Chemistry", "Mathematics"};
    printf("ID: %d, Name: %s\n", s->id, s->name);
    for (int j = 0; j < NUM_SUBJECTS; j++) {
        printf("  %s: %d\n", subjects[j], s->marks[j]);
    }
    printf("Average: %.2f, Grade: %c\n\n", s->average, s->grade);
}

// Clear terminal screen
void clearScreen() {
    printf("\033[H\033[2J\033[3J"); // clear and move cursor to top-left
    fflush(stdout);
}

// Pause until user presses enter
void pressEnter() {
    printf("\nPress enter to continue...");
    while (getchar() != '\n'); // check this
}

// Generic animation
void animateAction(const char *text, int dots, int delayUs) {
    printf("%s", text);
    fflush(stdout);
    for (int i = 0; i < dots; i++) {
        printf(".");
        fflush(stdout);
        usleep(delayUs);
    }
    printf("\r\033[K"); // clear line
}
