#ifndef BANK_H
#define BANK_H

#include <stdio.h>

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

typedef struct UserList {
    UserNode* head;
} UserList;

typedef struct Account {
    char* accountNumber;
    double balance;
    int userId;
} Account;

typedef struct AccountNode {
    Account account;
    struct AccountNode* next;
} AccountNode;

typedef struct AccountList {
    AccountNode* head;
} AccountList;

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

UserNode* createUserNode(char* username, char* hashedPassword, int id);
UserList* createUserList();
void addUser(UserList* list, UserNode* newUser);
void removeUser(UserList* list, char* username);
AccountNode* createAccountNode(char* accountNumber, double balance, int userId);
AccountList* createAccountList();
void addAccount(AccountList* list, AccountNode* newAccount);
void removeAccount(AccountList* list, char* accountNumber);

Transaction* createTransaction(char* operationCode, double amount, char* fromAccount, char* toAccount);
TransactionQueue* createTransactionQueue();
void enqueue(TransactionQueue* queue, Transaction* transaction);
Transaction* dequeue(TransactionQueue* queue);

char* hashPassword(const char* password);

void freeHashMap(HashMap* map);
void freeUserNode(UserNode* node);
void freeUserList(UserList* list);
void freeAccountNode(AccountNode* node);
void freeAccountList(AccountList* list);
void freeTransaction(Transaction* transaction);
void freeTransactionQueue(TransactionQueue* queue);

void writeUsersToFile(UserNode* user, FILE* file);
void writeAccountsToFile(AccountNode* account, FILE* file);
void writeTransactionsToFile(Transaction* transaction, FILE* file);

void registerUser(HashMap* map, UserList* users, char* username, char* password);
int loginUser(HashMap* map, UserList* users, char* username, char* password);

#endif
