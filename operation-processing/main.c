#include "operations.h"
#include "transaction_queue.h"
#include <stdio.h>
#include <sqlite3.h>

int main() 
{
    sqlite3 *db;
    if(sqlite3_open("bank.db",&db)) 
    {
        printf("\nError with opening the base");
        return 1;
    }
    deposit(db, "ivan", "200.50");
    withdraw(db, "ivan", "50.30");
    transfer(db, "ivan", "2", "25.00");
    printf("\nQueue for transfers:");
    print_transaction_queue();
    sqlite3_close(db);
    clear_transaction_queue();
    return 0;
}
