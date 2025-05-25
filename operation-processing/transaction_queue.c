#include "transaction_queue.h"
#include <stdio.h>
#include <stdlib.h>

static Transaction *head = NULL;
static Transaction *tail = NULL;
void enqueue_transaction(int from_id, int to_id, double amount) 
{
    Transaction *new_node = malloc(sizeof(Transaction));
    new_node->from_id = from_id;
    new_node->to_id = to_id;
    new_node->amount = amount;
    new_node->next = NULL;
    if(!tail) 
    {
        head = tail = new_node;
    } 
    else 
    {
        tail->next = new_node;
        tail = new_node;
    }
}

void print_transaction_queue(void) 
{
    Transaction *current = head;
    while(current) 
    {
        printf("Transfer:%d->%d | %.2f lv.\n",current->from_id, current->to_id, current->amount);
        current = current->next;
    }
}

void clear_transaction_queue(void) 
{
    Transaction *current = head;
    while(current) 
    {
        Transaction *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}