#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void handleMemoryError() {
    printf("Memory allocation failed\n");
    exit(1);
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

void insertEntry(HashMap* map, char* key, char* value) {
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
}

char* hashPassword(const char* password) {
    unsigned long hash = 4258;
    int c;
    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c;
    }
    char* hashedKey = (char*)malloc(20 * sizeof(char));
    if(!hashedKey) {
        handleMemoryError();
    }

    const char* base_chars = "432vdsfjkdhfds03";
    int base = strlen(base_chars);

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
        printf("User not found.\n");
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
        printf("Account not found.\n");
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


void writeUsersToFile(UserNode* user, FILE* file) {
    while (user != NULL) {
        fprintf(file, "%s %s %d\n", user->user.username, user->user.password, user->user.id);
        user = user->next;
    }
}

void writeAccountsToFile(AccountNode* account, FILE* file) {
    while (account != NULL) {
        fprintf(file, "%s %.2f %d\n", account->account.accountNumber, account->account.balance, account->account.userId);
        account = account->next;
    }
}

void writeTransactionsToFile(Transaction* transaction, FILE* file) {
    while (transaction != NULL) {
        fprintf(file, "%s %.2f %s %s\n", transaction->operationCode, transaction->amount, transaction->fromAccount, transaction->toAccount);
        transaction = transaction->next;
    }
}

void registerUser(HashMap* map, UserList* users, char* username, char* password) {
    if (getEntry(map, username) != NULL) {
        printf("Username already exists.\n");
        return;
    }

    char* hashedPassword = hashPassword(password);
    if(!hashedPassword) {
        printf("Error hashing password.\n");
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
        printf("Failed to create user node.\n");
        return;
    }

    addUser(users, newUser);
    map->userIdCounter++;

    printf("User '%s' registered successfully.\n", username);

    FILE* file = fopen("users.txt", "w");
    if (file) {
        writeUsersToFile(users->head, file);
        fclose(file);
    } else {
        printf("Error writing to file.\n");
    }

    free(hashedPassword);
}

int loginUser(HashMap* map, UserList* users, char* username, char* password) {
    char* hashedPassword = hashPassword(password);
    char* storedPassword = getEntry(map, username);

    if (storedPassword == NULL || strcmp(storedPassword, hashedPassword) != 0) {
        printf("Invalid username or password.\n");
        free(hashedPassword);
        return 0;
    }

    UserNode* current = users->head;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0) {
            printf("User '%s' logged in successfully.\n", username);
            free(hashedPassword);
            return 1;
        }
        current = current->next;
    }

    free(hashedPassword);
    return 0;
}
