#include "requests.h"

pthread_cond_t g_requests_onnew = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_requests_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t g_requests_rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

int
requests_handle_tokens(char **tokens, size_t count, int lockmode)
{
	pthread_mutex_t *mutex;
	request_t * request;
	void *data = NULL;
	int requestType;
	int i;

	if(count < 1)
		return 10;
	
	// Process type (first token) as appropriate
	if(strcmp("CHECK", tokens[0]) == 0)
	{
		int acct;
		requestType = REQUEST_CHECK;

		if(count < 2)
		{
			fprintf(stderr, "Usage: CHECK <accountid>");
			return 20;
		}
		sscanf(tokens[1], "%d", &acct);

		if(acct < 0 || acct >= g_accounts_count)
		{
			fprintf(stderr, "Invalid account id.");
			return 20;
		}

		// build check request
		data = request_check_new(acct);

		if(data == NULL)
		{
			fprintf(stderr, "Unable to create CHECK request.");
			return 20;
		}
	}
	else if(strcmp("TRANS", tokens[0]) == 0)
	{
		requestType = REQUEST_TRANS;

		if(count < 2 || (count-1)%2 != 0)
		{
			fprintf(stderr, "Usage: TRANS <acc1> <amount1> <acct2> <amount2> ...");
			return 21;
		}

		// Build a transaction request
		request_transaction_t *trans = request_transaction_new();

		if(trans == NULL)
		{
			fprintf(stderr, "Unabled to build TRANS.\n");
			return 21;
		}

		for(i = 1; i < count-1; i += 2)
		{
			int acct = -1;
			int amount = 0;
			int result = 0;

			sscanf(tokens[i], "%d", &acct);
			sscanf(tokens[i+1], "%d", &amount);

			if(acct < 0 || acct >= g_accounts_count)
			{
				fprintf(stderr, "Invalid account id.\n");
				request_transaction_destroy(trans);
				return 21;
			}

			result = request_transaction_add(trans, acct, amount);

			if(result <= 0)
			{
				fprintf(stderr, "Unable to create TRANS request.\n");
				request_transaction_destory(trans);
				return 21;
			}
		}


		data = trans;
	}
	else if(strcmp("END", tokens[0]) == 0)
	{
		requestType = REQUEST_END;
		// No other processing needed
		return 0;
	} else
		return 11; // Continue processing

	if(data == NULL)
		return -2;

	// Create new request
	request = requests_new();
	if(request == NULL)
		return -1;

	request->type = requestType;
	request->data = data;
	
	// Add to request list
	// mutex locking handled by list
	ll_push_back(g_requests, request);
	
	// Inform a worker of new request
	if(lockmode == LOCK_FINE)
		mutex = &g_requests_mutex;
	else // lockmode == LOCK_COARSE
		mutex = &g_mutex;
	if(0 == pthread_mutex_lock(mutex))
	{
		// TODO handle this somehow
		// request is in list, but no one knows about it
		return 0;
	}

	pthread_cond_signal(&g_requests_onnew);

	pthread_mutex_unlock(mutex);

	return 1;
}

void *
requests_worker(void *worked_optp)
{
	// TODO


	worker_opt_t *opt = (worker_opt_t*) worker_optp;



	// loop

	while(1)
	{

		// Wait for a request to process


		// Pop a request from the queue if possible


		if(opt->lockmode == LOCK_FINE)
			if(== pthread_rwlock_rdlock(g_requests_rwlock))
			{
				// TODO : Handle error
			}
		else
			if(== pthread_mutex_lock(g_mutex))
			{
				// TODO handle
			}


		// Process request, commiting changes

// Get read locks on account, else release all read locks
//  and wait for read or write to finish

// Check vector clocks on accounts, if less than request clocks
// Wait for writes to happen to the relevant accounts

// Do processing now that we have read locks on the correct acount vector clocks

// Wait for write locks, vector clocks guarantee we are the next write
// transaction to all relevant accounts

// Release all locks

		// Finished

	}

	return NULL;
}

request_t *
requests_new()
{
	request_t * request;
	static int requestId = 1;

	// Allocate memory
	request = malloc(sizeof(request_t));
	
	if(request == NULL)
		return NULL;

	request->id = requestId;
	request->type = REQUEST_INVALID;
	request->argc = 0;
	request->argv = NULL;
	
	requestId++;
	return request;
}

int
requests_create_workers(int count, int lockmode)
{

}

void
requests_handle(FILE *in, FILE *out, int workers, int accounts, char *prompt, int lockmode)
{
	// Initialize variables
	char *line = NULL;
	size_t linesize = 0;
	ssize_t read = 0;

	char **tokens = NULL;
	size_t tokensSize = 0;
	size_t tokenCount;

	int cont = 1;

	// Build a request list
	if(lockmode == LOCK_FINE)
		g_requests = ll_initialize(&g_requests_mutex);
	else // lockmode == LOCK_COARSE
		g_requests = ll_initialize(&g_mutex);
	if(g_requests == NULL)
		server_abort(ERROR_MEMORY_ALLOC);
	
	// Create bank accounts
	if(initialize_accounts(accounts) == 0)
		server_abort(ERROR_MEMORY_ALLOC);

	// Spin-up worker threads
	if(requests_create_workers(workers, lockmode) == 0)
		server_abort(ERROR_THREADING);

	// Read-parse-eval loop
	while (read != -1 && cont > 0)
	{

		// Read
		if(in == stdin)
			printf("%s", prompt);
		read = getline(&line, &linesize, in);

		// Scan
		tokenCount = tokLine(line, read, &tokens, &tokensSize);

		// Parse & eval
		cont = requests_handle_tokens(tokens, tokenCount, lockmode);

		// Cleanup
		clearTok(tokens, tokenCount);
	}

	// cleanup
	free(line);
	freeTok(tokens, tokenCount);
	ll_free(g_requests);
	// TODO: Workers
}

