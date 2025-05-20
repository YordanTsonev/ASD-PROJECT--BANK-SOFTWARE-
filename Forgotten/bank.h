#ifndef BANK_H
#define BANK_H

typedef struct EntryNode {
    char* key;
    char* value;
    struct EntryNode* next;
} EntryNode;

typedef struct HashMap {
    EntryNode** table;
    int size;
    int capacity;
    int userIdCounter;
} HashMap;

typedef struct User {
    char* username;
    char* password;
    int id;
} User;

typedef struct UserNode {
    User user;
    struct UserNode* next;
} UserNode;

typedef struct Account {
    char* accountNumber;
    double balance;
    int userId;
} Account;

typedef struct AccountNode {
    Account account;
    struct AccountNode* next;
} AccountNode;

typedef struct Transaction {
    char* operationCode;
    double amount;
    char* fromAccount;
    char* toAccount;
    struct Transaction* next;
} Transaction;

typedef struct TransactionQueue {
    Transaction* front;
    Transaction* rear;
} TransactionQueue;

EntryNode* createEntryNode(char* key, char* value);
HashMap* createHashMap(int capacity);
int hashFunction(HashMap* map, char* key);
char* getEntry(HashMap* map, char* key);
void insertEntry(HashMap* map, char* key, char* value);

UserNode* createUserNode(char* username, char* password, int id);
AccountNode* createAccountNode(char* accountNumber, double balance, int userId);

Transaction* createTransaction(char* operationCode, double amount, char* fromAccount, char* toAccount);
TransactionQueue* createTransactionQueue();
void enqueue(TransactionQueue* queue, Transaction* transaction);
Transaction* dequeue(TransactionQueue* queue);

char* hashPassword(const char* password);

void freeHashMap(HashMap* map);
void freeUserNode(UserNode* node);
void freeAccountNode(AccountNode* node);
void freeTransaction(Transaction* transaction);
void freeTransactionQueue(TransactionQueue* queue);

void registerUser(HashMap* map, char* username, char* password);

#endif
