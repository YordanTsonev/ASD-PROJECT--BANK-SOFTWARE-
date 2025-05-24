#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <sqlite3.h>

int deposit(sqlite3 *db, const char *username, const char *amount_str);
int withdraw(sqlite3 *db, const char *username, const char *amount_str);
int transfer(sqlite3 *db, const char *from_user, const char *to_account_id_str, const char *amount_str);

#endif 
