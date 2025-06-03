#include "bank.h"
#include "bank.c"
#include <stdlib.h>
#include <string.h>

int main() {
    HashMap* map = createHashMap(100);
    UserList* users = createUserList();
    AccountList* accounts = createAccountList();
    TransactionQueue* transactionQueue = createTransactionQueue();

    loadUsersFromFile(map, users, "users.txt");
    loadAccountsFromFile(accounts, "accounts.txt");
    loadTransactionsFromFile(transactionQueue, "transactions.txt");

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
                    int userId = getUserIdByUsername(users, username);
                    int action;
                    double amount;
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
                                printf("Enter amount to deposit: ");
                                scanf("%lf",&amount);
                                getchar();
                                deposit(accounts,userId,amount);
                                refreshAccountFile(accounts,"accounts.txt");
                                break;
                            case 2:
                                printf("Enter amount to withdraw: ");
                                scanf("%lf",&amount);
                                getchar();
                                withdraw(accounts, userId, amount);
                                refreshAccountFile(accounts, "accounts.txt");
                                break;
                            case 3:
                               char toAccount[ACCOUNT_NUMBER_LENGTH+1];
                                double amount;
                                printf("Enter addressee account number: ");
                                fgets(toAccount,ACCOUNT_NUMBER_LENGTH + 1,stdin);
                                toAccount[strcspn(toAccount,"\n")] = '\0';
                                getchar();
                                printf("Enter amount to transfer: ");
                                scanf("%lf",&amount);
                                getchar();
                                AccountNode* current = accounts->head;
                                char* fromAccount = NULL;
                                while(current!=NULL) 
                                {
                                    if(current->account.userId == userId) 
                                    {
                                        fromAccount = current->account.accountNumber;
                                        break;
                                    }
                                    current = current->next;
                                }
                                if (fromAccount) 
                                {
                                    transfer(fromAccount,toAccount,amount,accounts,transactionQueue);
                                    refreshTransactionFile(transactionQueue,"transactions.txt");
                                } 
                                else 
                                {
                                    printf("Your account was not found\n");
                                }
                                break;
                            
                            case 4:
                                executeTransaction(transactionQueue,accounts);
                                refreshAccountFile(accounts,"accounts.txt");
                                break;
                            case 5:
                                printf("Logging out...\n");
                                break;
                            default:
                                printf("Invalid action. Try again.\n");
                                break;
                        }
                    } while (action != 5);
                } 
                else 
                {
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

                registerUser(map, users, accounts, username, password, "users.txt", "accounts.txt");
                break;

            case 3:
                printf("Exiting the program...\n");
                refreshAccountFile(accounts,"accounts.txt");
                refreshUserFile(users,"users.txt");
                refreshTransactionFile(transactionQueue,"transactions.txt");
                freeHashMap(map);
                freeUserList(users);
                freeAccountList(accounts);
                freeTransactionQueue(transactionQueue);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}
