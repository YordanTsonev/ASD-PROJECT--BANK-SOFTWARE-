#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct user{
    char account_number[10];
    double current_balance;
    char id[50];
}User;

int find_account_by_index(User *user, int counter, const char *unique);
int load_accounts(const char filename, User **user, const char, int *counter);
int save_accounts(const char filename, User *user, const char, int *counter);


#endif 
