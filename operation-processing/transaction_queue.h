#ifndef TRANSACTION_QUEUE_H
#define TRANSACTION_QUEUE_H
typedef struct transaction{
    int from_id;
    int to_id;
    double amount;
    struct transaction *next;
}Transaction;

void enqueue_transaction(int from_id,int to_id, double amount);
void print_transaction_queue(void);
void clear_transaction_queue(void);

#endif
