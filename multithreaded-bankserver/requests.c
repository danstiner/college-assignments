#include "requests.h"

pthread_mutex_t g_requests_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

int
requests_handle_tokens(char **tokens, size_t count, int lockmode)
{
	request_t * request;
	int requestType;
	int i;

	if(count < 1)
		return 0;
	
	// Process type (first token) as appropriate
	if(strcmp("CHECK", tokens[0]) == 0)
	{
		requestType = REQUEST_CHECK;
	} else if(strcmp("TRANS", tokens[0]) == 0)
	{
		requestType = REQUEST_TRANS;
	} else if(strcmp("END", tokens[0]) == 0)
	{
		requestType = REQUEST_END;
	} else
		return 0;

	// Create new request
	request = requests_new();
	if(request == NULL)
		return -1;

	request->type = requestType;

	// Copy remaining tokens into argv of request
	request->argc = count - 1;
	request->argv = malloc(sizeof(char*) * request->argc);
	if(request->argv == NULL)
	{
		free(request);
		return -1;
	}
	for(i=0; i < count-1; ++i)
	{
		request->argv[i] = strdup(tokens[i+1]);
	}
	
	// Add to request list
	// mutex locking handled by list
	ll_push_back(g_requests, request);
	
	// Inform a worker of new request
	pthread_mutex_lock

	pthread_cond_signal

	pthread_mutex_unlock
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
	while (read != -1 && cont != 0)
	{

		// Read
		if(in == stdin)
			printf("%s", prompt);
		read = getline(&line, &linesize, in);

		// Parse
		tokenCount = tokLine(line, read, &tokens, &tokensSize);

		// Eval
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

