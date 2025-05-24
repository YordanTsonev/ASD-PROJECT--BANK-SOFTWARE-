#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_account_by_index(User *user, int counter, const char *unique)
{
    for(int i=0; i<counter; i++)
    {
        if(strcmp(user[i].account_number,unique)==0)
        {
            return i;
        }
    }
    return -1;
}

int load_accounts(const char *filename, User **user, const char, int *counter) 
{
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        return 0;
    }
    *counter = 0;
    *user = NULL;
    User var;
    while(fscanf(f, "%s %lf %s", var.account_number, &var.current_balance, var.id)==3)
    {
        User *new_array = realloc(*user,(*counter+1)*sizeof(User));
        if(!new_array) 
        {
            fclose(f);
            return 0;
        }
        *user = new_array;
        (*user)[*counter] = temp;
        (*counter)++;
    }
    fclose(f);
    return 1;
}


int save_accounts(const char *filename, User *user, int counter)
{
    FILE *f = fopen(filename, "w");
    if(!f) 
    {
        return 0;
    }
    for(int i=0;i<counter;i++) 
    {
        fprintf(f, "%s %.2f %s\n",user[i].account_number,user[i].current_balance,user[i].id);
    }
    fclose(f);
    return 1;
}