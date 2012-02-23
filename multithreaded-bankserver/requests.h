#ifndef REQUESTS_H_
#define REQUESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "linkedlist.h"
#include "Bank.h"

#define LOCK_FINE 0
#define LOCK_COARSE 1

#define REQUEST_INVALID 0
#define REQUEST_CHECK 1
#define REQUEST_TRANS 2
#define REQUEST_END 255


typedef struct request_item {
	int id;
	int type;
	int argc;
	char **argv;
} request_t;


ll_t *g_requests;

int requests_create_workers(int count, int lockmode);

int requests_handle_tokens(char **tokens, size_t count, int lockmode);

/**
 *  Handles user requests, blocks until user sends END request
 */
void requests_handle(FILE *in, FILE *out, int workers, int accounts, char *prompt, int lockmode);

#endif
