#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASSWORD_LENGTH 64
#define MAX_ATTEMPTS 3
#define DATA_FILE "account.dat"

// basic data model
typedef struct {
    char password[MAX_PASSWORD_LENGTH];
    int failed_attempts;
    int is_locked;
} Account;

void login_system(Account *account);
void initialize_account(Account *account);
void remove_newline(char *str);
int load_account(Account *account);
int save_account(Account *account);

int main(void) {
    Account account;

    if (!load_account(&account)) {
        initialize_account(&account);
    }

    login_system(&account);

    if(!save_account(&account)) {
        printf("Failed to save the account.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

void login_system(Account *account) {
    if (account -> is_locked) {
        printf("Account is locked. Access denied...\n");
        return;
    }

    char input[MAX_PASSWORD_LENGTH];

    while (!account -> is_locked) {
        printf("Enter the password: ");

        if (fgets(input, MAX_PASSWORD_LENGTH, stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }

        remove_newline(input);

        if (strcmp(input, account->password) == 0) {
            printf("Login successful!\n");
            account->failed_attempts = 0;
            break;
        }
        else {
            account->failed_attempts++;
            printf("Incorrect password. Attempts: %d/%d\n", account->failed_attempts, MAX_ATTEMPTS);

            if (account->failed_attempts >= MAX_ATTEMPTS) {
                account->is_locked = 1;
                printf("Account locked due to too many attempts.\n");
            }
        }
    }
}

void initialize_account(Account *account) {
    printf("Create a password (Max characters -> 64): ");
    if (fgets(account->password, MAX_PASSWORD_LENGTH, stdin) == NULL) {
        printf("Input error.\n");
        exit(EXIT_FAILURE);
    }

    remove_newline(account->password);
    account->failed_attempts = 0;
    account->is_locked = 0;

    printf("Account created successfully!\n");
}

int load_account(Account *account) {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        return 0;  // File doesn't exist
    }

    size_t read = fread(account, sizeof(Account), 1, file);
    fclose(file);

    return read == 1;
}

int save_account(Account *account) {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        return 0;
    }

    size_t written = fwrite(account, sizeof(Account), 1, file);
    fclose(file);

    return written == 1;
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}