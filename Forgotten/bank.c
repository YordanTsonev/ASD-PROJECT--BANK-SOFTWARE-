#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

static void handleMemoryError() {
    printf("Memory allocation failed\n");
    exit(1);
}

static char* generateAccountNumber() {
    char* accountNumber = (char*)malloc((ACCOUNT_NUMBER_LENGTH + 1) * sizeof(char));
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));

    for (int i = 0; i < ACCOUNT_NUMBER_LENGTH; i++) {
        int key = rand() % (sizeof(charset) - 1);
        accountNumber[i] = charset[key];
    }
    accountNumber[ACCOUNT_NUMBER_LENGTH] = '\0';

    return accountNumber;
}

EntryNode* createEntryNode(char* key, char* value) {
    EntryNode* newNode = (EntryNode*)malloc(sizeof(EntryNode));
    if (!newNode) {
        handleMemoryError();
    }
    newNode->key = strdup(key);
    if (!newNode->key) {
        free(newNode);
        handleMemoryError();
    }
    newNode->value = strdup(value);
    if (!newNode->value) {
        free(newNode->key);
        free(newNode);
        handleMemoryError();
    }
    newNode->next = NULL;
    return newNode;
}

HashMap* createHashMap(int capacity) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        handleMemoryError();
    }
    map->capacity = capacity;
    map->size = 0;
    map->userIdCounter = 1;
    map->table = (EntryNode**)malloc(capacity * sizeof(EntryNode*));
    if (!map->table) {
        free(map);
        handleMemoryError();
    }
    for (int i = 0; i < capacity; i++) {
        map->table[i] = NULL;
    }
    return map;
}

int hashFunction(HashMap* map, char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % map->capacity;
}

char* getEntry(HashMap* map, char* key) {
    int index = hashFunction(map, key);
    EntryNode* entry = map->table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

static int hashFunctionWithCapacity(char* key, int capacity) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

static void resizeHashMap(HashMap* map) {
    int newCapacity = map->capacity * 2;
    EntryNode** newTable = (EntryNode**)malloc(newCapacity * sizeof(EntryNode*));
    if (!newTable) {
        handleMemoryError();
    }

    for (int i = 0; i < newCapacity; i++) {
        newTable[i] = NULL;
    }

    for(int i = 0; i < map->capacity; i++) {
        EntryNode* entry = map->table[i];
        while (entry != NULL) {
            EntryNode* nextEntry = entry->next;

            int newIndex = hashFunctionWithCapacity(entry->key, newCapacity);
            entry->next = newTable[newIndex];
            newTable[newIndex] = entry;

            entry = nextEntry;
        }
    }

    free(map->table);
    map->table = newTable;
    map->capacity = newCapacity;
}

void insertEntry(HashMap* map, char* key, char* value) {
    if(map->size >= map->capacity) {
        resizeHashMap(map);
    }
    int index = hashFunction(map, key);
    EntryNode* current = map->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = strdup(value);
            if (!current->value) {
                handleMemoryError();
            }
            return;
        }
        current = current->next;
    }

    EntryNode* newNode = createEntryNode(key, value);
    newNode->next = map->table[index];
    map->table[index] = newNode;
    map->size++;
}

char* hashPassword(const char* password) {
    unsigned long hash = 4258;
    int c;
    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c;
    }

    const char* base_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int base = strlen(base_chars);
    
    char* hashedKey = (char*)malloc((base + 1) * sizeof(char));
    if(!hashedKey) {
        handleMemoryError();
    }

    hashedKey[base] = '\0';
    for (int i = base - 1; i >= 0; i--) {
       int digit = hash % base;
       hash = hash / base;
       hashedKey[i] = base_chars[digit];
    }

    return hashedKey;
}

UserNode* createUserNode(char* username, char* hashedPassword, int id) {
    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    if (!newNode) {
        handleMemoryError();
    }
    newNode->user.username = strdup(username);
    if (!newNode->user.username) {
        free(newNode);
        handleMemoryError();
    }
    newNode->user.password = strdup(hashedPassword);
    if (!newNode->user.password) {
        free(newNode->user.username);
        free(newNode);
        handleMemoryError();
    }
    newNode->user.id = id;
    newNode->next = NULL;
    return newNode;
}

UserList* createUserList() {
    UserList* list = (UserList*)malloc(sizeof(UserList));
    if (!list) {
        handleMemoryError();
    }
    list->head = NULL;
    return list;
}

void addUser(UserList* list, UserNode* newUser) {
    newUser->next = list->head;
    list->head = newUser;
}

void removeUser(UserList* list, char* username) {
    UserNode* current = list->head;
    UserNode* previous = NULL;

    while (current != NULL && strcmp(current->user.username, username) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("User not found\n");
        return;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current->user.username);
    free(current->user.password);
    free(current);
}

AccountNode* createAccountNode(char* accountNumber, double balance, int userId) {
    AccountNode* newNode = (AccountNode*)malloc(sizeof(AccountNode));
    if (!newNode) {
        handleMemoryError();
    }
    newNode->account.accountNumber = strdup(accountNumber);
    if (!newNode->account.accountNumber) {
        free(newNode);
        handleMemoryError();
    }
    newNode->account.balance = balance;
    newNode->account.userId = userId;
    newNode->next = NULL;
    return newNode;
}

AccountList* createAccountList() {
    AccountList* list = (AccountList*)malloc(sizeof(AccountList));
    if (!list) {
        handleMemoryError();
    }
    list->head = NULL;
    return list;
}

void addAccount(AccountList* list, AccountNode* newAccount) {
    newAccount->next = list->head;
    list->head = newAccount;
}

void removeAccount(AccountList* list, char* accountNumber) {
    AccountNode* current = list->head;
    AccountNode* previous = NULL;

    while (current != NULL && strcmp(current->account.accountNumber, accountNumber) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Account not found\n");
        return;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current->account.accountNumber);
    free(current);
}

Transaction* createTransaction(char* operationCode, double amount, char* fromAccount, char* toAccount) {
    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
    if (!newTransaction) {
        handleMemoryError();
    }
    newTransaction->operationCode = strdup(operationCode);
    if (!newTransaction->operationCode) {
        free(newTransaction);
        handleMemoryError();
    }
    newTransaction->amount = amount;
    newTransaction->fromAccount = strdup(fromAccount);
    if (!newTransaction->fromAccount) {
        free(newTransaction->operationCode);
        free(newTransaction);
        handleMemoryError();
    }
    newTransaction->toAccount = strdup(toAccount);
    if (!newTransaction->toAccount) {
        free(newTransaction->operationCode);
        free(newTransaction->fromAccount);
        free(newTransaction);
        handleMemoryError();
    }
    newTransaction->next = NULL;
    return newTransaction;
}

TransactionQueue* createTransactionQueue() {
    TransactionQueue* queue = (TransactionQueue*)malloc(sizeof(TransactionQueue));
    if (!queue) {
        handleMemoryError();
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(TransactionQueue* queue, Transaction* transaction) {
    if (queue->rear == NULL) {
        queue->front = transaction;
        queue->rear = transaction;
    } else {
        queue->rear->next = transaction;
        queue->rear = transaction;
    }
}

Transaction* dequeue(TransactionQueue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }

    Transaction* temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    return temp;
}

void freeHashMap(HashMap* map) {
    for (int i = 0; i < map->capacity; i++) {
        EntryNode* entry = map->table[i];
        while (entry != NULL) {
            EntryNode* temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

void freeUserNode(UserNode* node) {
    free(node->user.username);
    free(node->user.password);
    free(node);
}

void freeUserList(UserList* list) {
    UserNode* current = list->head;
    while (current != NULL) {
        UserNode* temp = current;
        current = current->next;
        freeUserNode(temp);
    }
    free(list);
}

void freeAccountNode(AccountNode* node) {
    free(node->account.accountNumber);
    free(node);
}

void freeAccountList(AccountList* list) {
    AccountNode* current = list->head;
    while (current != NULL) {
        AccountNode* temp = current;
        current = current->next;
        freeAccountNode(temp);
    }
    free(list);
}

void freeTransaction(Transaction* transaction) {
    free(transaction->operationCode);
    free(transaction->fromAccount);
    free(transaction->toAccount);
    free(transaction);
}

void freeTransactionQueue(TransactionQueue* queue) {
    Transaction* current = queue->front;
    while (current != NULL) {
        Transaction* temp = current;
        current = current->next;
        freeTransaction(temp);
    }
    free(queue);
}

void registerUser(HashMap* map, UserList* users, AccountList* accounts, char* username, char* password, const char* filenameUsers, const char* filenameAccounts) {
    if (getEntry(map, username) != NULL) {
        printf("Username already exists\n");
        return;
    }

    char* hashedPassword = hashPassword(password);
    if(!hashedPassword) {
        printf("Error hashing password\n");
        return;
    }
    insertEntry(map, username, hashedPassword);
    UserNode* newUser = createUserNode(username, hashedPassword, map->userIdCounter);
    if (!newUser) {
        int index = hashFunction(map, username);
        EntryNode* prev = NULL;
        EntryNode* curr = map->table[index];
        while (curr != NULL) {
            if (strcmp(curr->key, username) == 0) {
                if (prev == NULL) {
                    map->table[index] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                free(curr->key);
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        free(hashedPassword);
        printf("Failed to create user node\n");
        return;
    }

    addUser(users, newUser);
    map->userIdCounter++;

    AccountNode* newAccount = createAccountNode(strdup(generateAccountNumber()), 0.0, newUser->user.id);
    addAccount(accounts, newAccount);

    printf("User '%s' registered successfully\n", username);

    refreshUserFile(users, filenameUsers);
    refreshAccountFile(accounts, filenameAccounts);

    free(hashedPassword);
}

int loginUser(HashMap* map, UserList* users, char* username, char* password) {
    char* hashedPassword = hashPassword(password);
    char* storedPassword = getEntry(map, username);

    if (storedPassword == NULL || strcmp(storedPassword, hashedPassword) != 0) {
        printf("Invalid username or password\n");
        free(hashedPassword);
        return 0;
    }

    UserNode* current = users->head;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0) {
            printf("User '%s' logged in successfully\n", username);
            free(hashedPassword);
            return 1;
        }
        current = current->next;
    }

    free(hashedPassword);
    return 0;
}

void transfer(char* fromAccount, char* toAccount, double amount, AccountList* accounts, TransactionQueue* queue) {
    if (amount <= 0) {
        printf("Invalid amount! Must be a positive number!\n");
        return;
    }

    int fromFound = 0, toFound = 0;
    AccountNode* cur = accounts->head;

    while (cur != NULL) {
        if (strcmp(cur->account.accountNumber, fromAccount) == 0) {
            if(cur->account.balance < amount) {
                printf("Insufficient balance in account '%s'\n", fromAccount);
                return;
            }
            fromFound = 1;
        } else if (strcmp(cur->account.accountNumber, toAccount) == 0) {
            toFound = 1;
        }
        cur = cur->next;
    }

    if (!fromFound && !toFound) {
        printf("Both accounts '%s' and '%s' not found\n", fromAccount, toAccount);
        return;
    } else if (!fromFound) {
        printf("Source account '%s' not found\n", fromAccount);
        return;
    } else if (!toFound) {
        printf("Destination account '%s' not found\n", toAccount);
        return;
    }

    Transaction* transaction = createTransaction("transfer", amount, fromAccount, toAccount);
    if (!transaction) {
        printf("Failed to create transaction\n");
        return;
    }

    enqueue(queue, transaction);
    printf("Transaction added to queue: %.2f from '%s' to '%s'\n", amount, fromAccount, toAccount);
}

void executeTransaction(TransactionQueue* queue, AccountList* accounts) {
    if (queue->front == NULL) {
        printf("No transactions to execute\n");
        return;
    }

    while(queue->front != NULL) {
        Transaction* transaction = dequeue(queue);

        AccountNode* fromAccountNode = NULL;
        AccountNode* toAccountNode = NULL;

        AccountNode* cur = accounts->head;
        while (cur != NULL) {
            if (strcmp(cur->account.accountNumber, transaction->fromAccount) == 0) {
                fromAccountNode = cur;
            } else if (strcmp(cur->account.accountNumber, transaction->toAccount) == 0) {
                toAccountNode = cur;
            }
            cur = cur->next;
        }

        if(transaction->amount <= 0) {
            printf("Invalid transaction amount: %.2f\n", transaction->amount);
            freeTransaction(transaction);
            return;
        }

        if (fromAccountNode && toAccountNode) {
            if(fromAccountNode->account.balance < transaction->amount) {
                printf("Insufficient balance in account '%s' for transaction of %.2f\n", transaction->fromAccount, transaction->amount);
                freeTransaction(transaction);
                return;
            }
            fromAccountNode->account.balance -= transaction->amount;
            toAccountNode->account.balance += transaction->amount;

            printf("Executed transaction: %.2f from '%s' to '%s'\n", transaction->amount, transaction->fromAccount, transaction->toAccount);
        } else {
            printf("Transaction failed: one or both accounts not found.\n");
        }

        freeTransaction(transaction);
    }
}

void loadUsersFromFile(HashMap* map, UserList* users, const char* filename) {
    if (filename && strlen(filename) > 0) {
        FILE* usersFile = fopen(filename, "r");
        if(usersFile){
            char line[LINE_LENGTH];
            while (fgets(line, sizeof(line), usersFile)) {
                line[strcspn(line, "\n")] = '\0';

                char* username = strtok(line, " ");
                char* hashedPassword = strtok(NULL, " ");
                char* userIdStr = strtok(NULL, " ");

                if(username && hashedPassword && userIdStr) {
                    int id = atoi(userIdStr);

                    insertEntry(map, username, hashedPassword);

                    UserNode* newUser = createUserNode(username, hashedPassword, id);
                    addUser(users, newUser);

                    if(id >= map->userIdCounter) {
                        map->userIdCounter = id + 1;
                    }
                }
                else {
                    printf("Invalid user entry: %s\n", line);
                }
            }
            fclose(usersFile);
        }
        else {
            printf("Failed to open file: %s\n", filename);
        }
    }
    else {
        printf("Filename is empty or NULL\n");
    }
}

void loadAccountsFromFile(AccountList* accounts, const char* filename) {
    if (filename && strlen(filename) > 0) {
        FILE* accountsFile = fopen(filename, "r");
        if(accountsFile) {
            char line[LINE_LENGTH];
            while (fgets(line, sizeof(line), accountsFile)) {
                line[strcspn(line, "\n")] = '\0';

                char* accountNumber = strtok(line, " ");
                char* balanceStr = strtok(NULL, " ");
                char* userIdStr = strtok(NULL, " ");

                if(accountNumber && balanceStr && userIdStr) {
                    double balance = atof(balanceStr);
                    int userId = atoi(userIdStr);

                    AccountNode* newAccount = createAccountNode(accountNumber, balance, userId);
                    addAccount(accounts, newAccount);
                }
                else {
                    printf("Invalid account entry: %s\n", line);
                }
            }
            fclose(accountsFile);
        }
        else {
            printf("Failed to open file: %s\n", filename);
        }
    }
    else {
        printf("Filename is empty or NULL\n");
    }
}

void loadTransactionsFromFile(TransactionQueue* queue, const char* filename) {
    if(filename && strlen(filename) > 0) {
        FILE* transactionsFile = fopen(filename, "r");
        if (transactionsFile) {
            char line[LINE_LENGTH];
            while (fgets(line, sizeof(line), transactionsFile)) {
                line[strcspn(line, "\n")] = '\0';

                char* operationCode = strtok(line, " ");
                char* amountStr = strtok(NULL, " ");
                char* fromAccount = strtok(NULL, " ");
                char* toAccount = strtok(NULL, " ");

                if(operationCode && amountStr && fromAccount && toAccount) {
                    double amount = atof(amountStr);
                    Transaction* newTransaction = createTransaction(operationCode, amount, fromAccount, toAccount);
                    enqueue(queue, newTransaction);
                }
                else {
                    printf("Invalid transaction entry: %s\n", line);
                }
            }
            fclose(transactionsFile);
        }
        else {
            printf("Failed to open file: %s\n", filename);
        }
    }
    else {
        printf("Filename is empty or NULL\n");
    }
}

void refreshUserFile(UserList* users, const char* filename) {
    if (filename == NULL || strlen(filename) == 0) {
        printf("Filename is empty or NULL\n");
        return;
    }
    FILE* file = fopen(filename, "w");
    if (file) {
        UserNode* current = users->head;
        while (current != NULL) {
            fprintf(file, "%s %s %d\n", current->user.username, current->user.password, current->user.id);
            current = current->next;
        }
        fclose(file);
    } else {
        printf("Error writing to users file\n");
    }
}

void refreshAccountFile(AccountList* accounts, const char* filename) {
    if (filename == NULL || strlen(filename) == 0) {
        printf("Filename is empty or NULL\n");
        return;
    }
    FILE* file = fopen(filename, "w");
    if (file) {
        AccountNode* current = accounts->head;
        while (current != NULL) {
            fprintf(file, "%s %.2f %d\n", current->account.accountNumber, current->account.balance, current->account.userId);
            current = current->next;
        }
        fclose(file);
    } else {
        printf("Error writing to accounts file\n");
    }
}

void refreshTransactionFile(TransactionQueue* queue, const char* filename) {
    if (filename == NULL || strlen(filename) == 0) {
        printf("Filename is empty or NULL\n");
        return;
    }
    FILE* file = fopen(filename, "w");
    if (file) {
        Transaction* current = queue->front;
        while (current != NULL) {
            fprintf(file, "%s %.2f %s %s\n", current->operationCode, current->amount, current->fromAccount, current->toAccount);
            current = current->next;
        }
        fclose(file);
    } else {
        printf("Error writing to transactions file\n");
    }
}