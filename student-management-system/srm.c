// This is a simple student record management program
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_SUBJECTS 3
#define INITIAL_CAPACITY 2
#define FILENAME "students.txt"
#define MENU_MIN 0
#define MENU_MAX 4

// Color codes
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

// Data model
typedef struct {
    int id;
    char name[100];
    int marks[NUM_SUBJECTS];
    float average;
    char grade;
} Student;

// Function prototypes
void ensureCapacity(Student **arr, int *size, int *capacity);
int idExists(Student *arr, int size, int id);
void inputStudent(Student *arr, int index, int *size);
void calculateResults(Student *arr, int index);
void saveStudents(Student *arr, int size, const char *filename);
void loadStudents(Student **arr, int *size, int *capacity, const char *filename);
void printStudent(const Student *s);
void deleteStudent(Student *arr, int *size);
void sortStudents(Student *arr, int size);
void animateAction(const char *text, int dots, int delayUs);
void pressEnter();
void clearScreen();

int main(void) {

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

// Utility
void printStudent(const Student *s) {
    char subjects[NUM_SUBJECTS][20] = {"Physics", "Chemistry", "Mathematics"};
    printf("ID: %d, Name: %s\n", s->id, s->name);
    for (int j = 0; j < NUM_SUBJECTS; j++) {
        printf("  %s: %d\n", subjects[j], s->marks[j]);
    }
    printf("Average: %.2f, Grade: %c\n\n", s->average, s->grade);
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

// Pause until user presses enter
void pressEnter() {
    printf("\nPress enter to continue...");
    while (getchar() != '\n'); // check this
}

// Clear terminal screen
void clearScreen() {
    printf("\033[H\033[2J\033[3J"); // clear and move cursor to top-left
    fflush(stdout);
}

