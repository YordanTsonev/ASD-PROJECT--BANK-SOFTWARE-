#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct account{
    char account_number[10];
    double current_balance;
    char id[50];
}Account;

int find_account_by_index(Account *account, int counter, const char *unique);
int load_accounts(const char *filename, Account **account, const char, int *counter);
int save_accounts(const char *filename, Account *account, const char, int counter);


#endif 
