#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#include "tokenizer.h"
#include "linkedlist.h"
#include "main.h"

#define LOCK_FINE 0
#define LOCK_COARSE 1

#define REQUEST_RESULT_FAIL 0
#define REQUEST_RESULT_RETRY -777
#define REQUEST_RESULT_SUCCESS 1


typedef struct request_item {
	int id;
	int type;
	void *data;
	struct timeval start;
} request_t;

typedef struct request_handle_options {
	app_options_t *appoptions;
	ll_t *requests;

	int stop;

	pthread_mutex_t requests_mutex;
	pthread_mutex_t request_list_mutex;
	pthread_mutex_t coarse_mutex;

	pthread_cond_t requests_onnew;
} request_handle_opt_t;

typedef struct worker_options_struct {
	int id;

	app_options_t *appopt;
	request_handle_opt_t *handleopt;
} worker_opt_t;


#define REQUEST_INVALID 0
#define REQUEST_CHECK 1
#define REQUEST_TRANSACTION 2
#define REQUEST_END 255

// Must be after request_t as both use that type
#include "request_check.h"
#include "request_transaction.h"


/**
 *  Handles user requests, blocks until user sends END request
 */
void requests_handle(app_options_t *options);


// private

request_t *requests_new();

void *requests_worker(void *worker_optp);

pthread_t *requests_create_workers(request_handle_opt_t *options);

void requests_join_workers(pthread_t ** workers, request_handle_opt_t *options);

int requests_handle_tokens(char **tokens, size_t count, request_handle_opt_t *options);



#endif
