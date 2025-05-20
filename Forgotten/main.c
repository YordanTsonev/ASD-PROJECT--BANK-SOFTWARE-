#include "bank.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    while(1) {
        printf("Choose an option:\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Login selected.\n");
                printf("Hello, %s\n", "User");
                printf("What would you like to do?\n");
                printf("1. Deposit\n");
                printf("2. Withdraw\n");
                printf("3. Transfer\n");
                printf("4. View Transactions\n");
                printf("5. Logout\n");

                int action;
                scanf("%d", &action);
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
                        printf("Logout selected.\n");
                        break;
                    default:
                        printf("Invalid action. Please try again.\n");
                        break;
                }

                break;
            case 2:
                printf("Register selected.\n");
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    return 0;
}