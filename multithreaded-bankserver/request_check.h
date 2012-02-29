#ifndef REQUEST_CHECK_H
#define REQUEST_CHECK_H

#include "requests.h"
#include "main.h"
#include "accounts.h"


typedef struct request_check_struct
{
	int account;
	int account_clock;
} request_check_t;


int request_check_is(char **tokens, int count);

int request_check_isr(request_t *request);

int request_check_create(char **tokens, int count, request_t *request, app_options_t *options);

int request_check_process(request_t *request, app_options_t *options);

// private
request_check_t *request_check_new();

int request_check_destory(request_check_t *transaction);

#endif
