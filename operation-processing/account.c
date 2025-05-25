#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_account_by_index(Account *account, int counter, const char *unique)
{
    for(int i=0; i<counter; i++)
    {
        if(strcmp(account[i].account_number,unique)==0)
        {
            return i;
        }
    }
    return -1;
}

int load_accounts(const char *filename, Account **account, const char, int *counter) 
{
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        return 0;
    }
    *counter = 0;
    *account = NULL;
    Account var;
    while(fscanf(f, "%s %lf %s", var.account_number, &var.current_balance, var.id)==3)
    {
        Account *new_array = realloc(*account,(*counter+1)*sizeof(Account));
        if(!new_array) 
        {
            fclose(f);
            return 0;
        }
        *account = new_array;
        (*account)[*counter] = var;
        (*counter)++;
    }
    fclose(f);
    return 1;
}


int save_accounts(const char *filename, Account *account, int counter)
{
    FILE *f = fopen(filename, "w");
    if(!f) 
    {
        return 0;
    }
    for(int i=0;i<counter;i++) 
    {
        fprintf(f, "%s %.2f %s\n",account[i].account_number,account[i].current_balance,account[i].id);
    }
    fclose(f);
    return 1;
}
