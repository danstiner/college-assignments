#ifndef REQUEST_TRANSACTION_H
#define REQUEST_TRANSACTION_H

#include "common.h"
#include "requests.h"
#include "main.h"
#include "accounts.h"


#define TRANSACTION_INSUFFICIENT_BALANCE -42


typedef struct request_transaction_part_struct
{
	int account;
	int account_clock;
	int amount;

	struct request_transaction_part_struct *next;
} request_transaction_part_t;

typedef struct request_transaction_struct
{
	int count;
	request_transaction_part_t *parts;
} request_transaction_t;


int request_transaction_is(char **tokens, int count);

int request_transaction_isr(request_t *request);

int request_transaction_create(char **tokens, int count, request_t *request, app_options_t *options);

int request_transaction_process(request_t *request, app_options_t *options);

// private

int request_transaction_process_internal(request_transaction_t *request, request_t *prequest, app_options_t *options);

request_transaction_t *request_transaction_new();

int request_transaction_add(request_transaction_t *transaction, int account, int amount, app_options_t *options);

int request_transaction_destory(request_transaction_t **transaction);

#endif
