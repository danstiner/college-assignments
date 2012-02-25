#ifndef REQUEST_TRANSACTION_H
#define REQUEST_TRANSACTION_H

typedef struct request_transaction_struct
{
	
} request_transaction_t;


request_transaction_t *request_transaction_new();

int request_transaction_add(request_transaction_t *transaction, int account, int amount);

int request_transaction_destory(request_transaction_t *transaction);

#endif
