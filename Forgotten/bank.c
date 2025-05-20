#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EntryNode* createEntryNode(char* key, char* value) {
    EntryNode* newNode = (EntryNode*)malloc(sizeof(EntryNode));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;
    return newNode;
}

HashMap* createHashMap(int capacity) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->capacity = capacity;
    map->size = 0;
    map->userIdCounter = 1;
    map->table = (EntryNode**)malloc(capacity * sizeof(EntryNode*));
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
    EntryNode* newNode = createEntryNode(key, value);
    newNode->next = map->table[index];
    map->table[index] = newNode;
}

char* hashPassword(const char* password) {
    unsigned long hash = 5381;
    int c;
    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c;
    }
    char* result = (char*)malloc(21);
    snprintf(result, 21, "%lx", hash);
    return result;
}

UserNode* createUserNode(char* username, char* password, int id) {
    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    newNode->user.username = strdup(username);
    newNode->user.password = hashPassword(password);
    newNode->user.id = id;
    newNode->next = NULL;
    return newNode;
}

AccountNode* createAccountNode(char* accountNumber, double balance, int userId) {
    AccountNode* newNode = (AccountNode*)malloc(sizeof(AccountNode));
    newNode->account.accountNumber = strdup(accountNumber);
    newNode->account.balance = balance;
    newNode->account.userId = userId;
    newNode->next = NULL;
    return newNode;
}

Transaction* createTransaction(char* operationCode, double amount, char* fromAccount, char* toAccount) {
    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
    newTransaction->operationCode = strdup(operationCode);
    newTransaction->amount = amount;
    newTransaction->fromAccount = strdup(fromAccount);
    newTransaction->toAccount = strdup(toAccount);
    newTransaction->next = NULL;
    return newTransaction;
}

TransactionQueue* createTransactionQueue() {
    TransactionQueue* queue = (TransactionQueue*)malloc(sizeof(TransactionQueue));
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
    for (int i = 0; i < map->size; i++) {
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

void freeAccountNode(AccountNode* node) {
    free(node->account.accountNumber);
    free(node);
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

void registerUser(HashMap* map, char* username, char* password) {
    if (getEntry(map, username) != NULL) {
        printf("User already exists.\n");
        return;
    }

    UserNode* newUser = createUserNode(username, password, map->userIdCounter++);
    char* hashed = strdup(newUser->user.password);
    insertEntry(map, username, hashed);

    printf("User '%s' registered with ID %d.\n", newUser->user.username, newUser->user.id);
    freeUserNode(newUser);
}