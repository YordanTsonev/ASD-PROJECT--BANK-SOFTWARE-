#include "bank.h"
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 256

int main() {
    HashMap* map = createHashMap(100);
    UserList* users = createUserList();

    char username[MAX_INPUT];
    char password[MAX_INPUT];

    while (1) {
        printf("\nWelcome to the Bank System!\n");
        printf("Choose an option:\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Choice: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Username: ");
                fgets(username, MAX_INPUT, stdin);
                username[strcspn(username, "\n")] = '\0';

                printf("Password: ");
                fgets(password, MAX_INPUT, stdin);
                password[strcspn(password, "\n")] = '\0';

                if (loginUser(map, users, username, password)) {
                    int action;
                    do {
                        printf("\nWelcome, %s!\n", username);
                        printf("Choose an action:\n");
                        printf("1. Deposit\n");
                        printf("2. Withdraw\n");
                        printf("3. Transfer\n");
                        printf("4. View Transactions\n");
                        printf("5. Logout\n");
                        printf("Choice: ");
                        scanf("%d", &action);
                        getchar();

                        switch (action) {
                            case 1:
                                printf("Deposit selected.\n");
                                
                                break;
                            case 2:
                                printf("Withdraw selected.\n");
                                
                                break;
                            case 3:
                                printf("Transfer selected.\n");
                                
                                break;
                            case 4:
                                printf("View Transactions selected.\n");
                                
                                break;
                            case 5:
                                printf("Logging out...\n");
                                break;
                            default:
                                printf("Invalid action. Try again.\n");
                                break;
                        }
                    } while (action != 5);
                } else {
                    printf("Login failed.\n");
                }
                break;

            case 2:
                printf("\nRegister:\n");
                printf("Choose a username: ");
                fgets(username, MAX_INPUT, stdin);
                username[strcspn(username, "\n")] = '\0';

                printf("Choose a password: ");
                fgets(password, MAX_INPUT, stdin);
                password[strcspn(password, "\n")] = '\0';

                registerUser(map, users, username, password);
                break;

            case 3:
                printf("Exiting...\n");
                freeHashMap(map);
                freeUserList(users);
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}
