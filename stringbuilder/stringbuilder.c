#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void push_back(char **arr, int *size, int *capacity, char character);

int main(void) {
    int size = 0, capacity = 1;

    char *string = malloc(capacity * sizeof(char));
    if (string == NULL) {
        return 1;
    }

    // read characters one by one
    printf("Enter a string: ");
    while(true) {
        char ch;
        scanf("%c", &ch);
        if (ch == '\n') {
            break;
        }
        else {
            push_back(&string, &size, &capacity, ch);
        }
    }

    *(string + size) = '\0'; // add the null character at the end

    // print the characters
    printf("You entered: %s\n", string);

    free(string);
    return 0;

}

void push_back(char **arr, int *size, int *capacity, char character) {
    if (*size == *capacity)
    {
        *capacity *= 2;
        char *temp = realloc(*arr, (*capacity) * sizeof(char));
        if (temp == NULL) {
            return;
        }

        *arr = temp;
    }

    (*arr)[*size] = character;
    (*size)++;
}