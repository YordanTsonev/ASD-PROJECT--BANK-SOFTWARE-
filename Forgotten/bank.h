#ifndef BANK_H
#define BANK_H

#include <stdio.h>

#define MAX_INPUT 256
#define LINE_LENGTH 1024
#define ACCOUNT_NUMBER_LENGTH 16

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

void registerUser(HashMap* map, UserList* users, AccountList* accounts, char* username, char* password, const char* filenameUsers, const char* filenameAccounts);
int loginUser(HashMap* map, UserList* users, char* username, char* password);
void transfer(char* fromAccount, char* toAccount, double amount, AccountList* accounts, TransactionQueue* queue, const char* filename);
void executeTransaction(TransactionQueue* queue, AccountList* accounts, const char* Tfilename, const char* Afilename);
void viewTransactions(TransactionQueue* queue);

void loadUsersFromFile(HashMap* map, UserList* users, const char* filename);
void loadAccountsFromFile(AccountList* accounts, const char* filename);
void loadTransactionsFromFile(TransactionQueue* queue, const char* filename);

void refreshUserFile(UserList* users, const char* filename);
void refreshAccountFile(AccountList* accounts, const char* filename);
void refreshTransactionFile(TransactionQueue* queue, const char* filename);

void deposit(AccountList* accounts, int userId, double amount, const char* filename);
void withdraw(AccountList* accounts, int userId, double amount, const char* filename);
int getUserIdByUsername(UserList* users, const char* username);


#endif
