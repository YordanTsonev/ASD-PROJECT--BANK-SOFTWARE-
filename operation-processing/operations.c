#include "operations.h"
#include "transaction_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int validate_amount(const char *str, double *out) 
{
    char *endptr;
    *out = strtod(str, &endptr);
    if(*str=='\0'||*endptr!='\0'||*out<= 0)return 0;
    const char *dot = strchr(str, '.');
    if(dot && strlen(dot+1)>2)return 0;
    return 1;
}

static int get_user(sqlite3 *db, const char *username, double *balance, int *user_id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, balance FROM users WHERE username = ?";
    if(sqlite3_prepare_v2(db,sql,-1,&stmt, NULL)!=SQLITE_OK)return 0;
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    int found = 0;
    if(sqlite3_step(stmt)==SQLITE_ROW) 
    {
        *user_id = sqlite3_column_int(stmt, 0);
        *balance = sqlite3_column_double(stmt, 1);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}

static int get_user_by_id(sqlite3 *db, int id, double *balance) 
{
   sqlite3_stmt *stmt;
    const char *sql = "SELECT balance FROM users WHERE id = ?";
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)return 0;
    sqlite3_bind_int(stmt, 1, id);
    int found = 0;
    if(sqlite3_step(stmt)==SQLITE_ROW) 
    {
        *balance = sqlite3_column_double(stmt, 0);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}

static int update_balance(sqlite3 *db, int user_id, double new_balance) 
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE users SET balance = ? WHERE id = ?";
    if(sqlite3_prepare_v2(db,sql,-1,&stmt,NULL)! SQLITE_OK)return 0;
    sqlite3_bind_double(stmt,1,new_balance);
    sqlite3_bind_int(stmt,2,user_id);
    int result = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return result;
}

int deposit(sqlite3 *db, const char *username, const char *amount_str) 
{
    double amount;
    if(!validate_amount(amount_str,&amount)) 
    {
        printf("\nInvalid amount for deposit");
        return 0;
    }
    double balance;
    int user_id;
    if(!get_user(db,username,&balance,&user_id)) 
    {
        printf("\nThe user doesn't exist");
        return 0;
    }
    balance += amount;
    if(update_balance(db,user_id,balance)) 
    {
        printf("\nSuccessful deposit: +%.2f lv", amount);
        return 1;
    }
    return 0;
}

int withdraw(sqlite3 *db, const char *username, const char *amount_str) 
{
    double amount;
    if(!validate_amount(amount_str,&amount)) 
    {
        printf("\nInvalid amount for withdraw");
        return 0;
    }
    double balance;
    int user_id;
    if(!get_user(db, username,&balance,&user_id)) 
    {
        printf("\nThe user doesn't exist");
        return 0;
    }
    if(balance < amount) 
    {
        printf("\nThe balance isn't enough");
        return 0;
    }
    balance -= amount;
    if(update_balance(db,user_id,balance)) 
    {
        printf("\nSuccessful withdraw: -%.2f lv", amount);
        return 1;
    }
    return 0;
}

int transfer(sqlite3 *db, const char *from_user, const char *to_account_id_str, const char *amount_str) 
{
    double amount;
    if(!validate_amount(amount_str, &amount)) 
    {
        printf("\nInvalid amount for transfer");
        return 0;
    }
    int to_id = atoi(to_account_id_str);
    double to_balance;
    if(!get_user_by_id(db,to_id,&to_balance)) 
    {
        printf("\nThe account doesn't exist");
        return 0;
    }
    double from_balance;
    int from_id;
    if (!get_user(db,from_user,&from_balance,&from_id)) 
    {
        printf("\nТhe sender doesn't exist");
        return 0;
    }

    if (from_balance < amount) 
    {
        printf("\nThe balance for transfer isn't enough");
        return 0;
    }
    enqueue_transaction(from_id,to_id,amount);
    printf("The transfer is added to the queue: %.2f lv. from %s towards %d\n", amount,from_user,to_id);
    return 1;
}