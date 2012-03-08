#include "requests.h"

int
requests_handle_tokens(char **tokens, size_t count, request_handle_opt_t *options)
{
	pthread_mutex_t *mutex;
	request_t * request;

	if(count < 1)
		return 10;

	// Create new request
	request = requests_new();
	if(request == NULL)
		return -1;

	// Process type (first token) as appropriate
	if(request_check_is(tokens, count))
	{
		request_check_create(tokens, count, request, options->appoptions);
	}
	else if(request_transaction_is(tokens, count))
	{
		request_transaction_create(tokens, count, request, options->appoptions);
	}
	else if(strcmp("END", tokens[0]) == 0)
	{
		request->type = REQUEST_END;
		// No other processing needed
		return 0;
	} else
		return 11; // Continue processing

	if(request->type == REQUEST_INVALID)
	{
		// TODO
		return 12;
	}

	// Print transaction ID back to console
	fprintf(stdout, "ID %d\n", request->id);


	// Lock coarse mutex for entire insert
	if(options->appoptions->lockmode == LOCK_COARSE)
		pthread_mutex_lock(&options->coarse_mutex);

	// Add to request list
	// fine mutex locking handled by list
	ll_push_back(options->requests, request);
	
	// Inform a worker of new request
	if(options->appoptions->lockmode == LOCK_FINE)
	{
		mutex = &options->requests_mutex;
		pthread_mutex_lock(mutex);
	}
	else // lockmode == LOCK_COARSE
		mutex = &options->coarse_mutex;
	
	// Signal workers that there is a new request
	pthread_cond_signal(&options->requests_onnew);

	// Unlock and finish
	pthread_mutex_unlock(mutex);

	return 1;
}

void *
requests_worker(void *worker_optp)
{
	int empty = 1;
	int result;
	request_t *request;
	pthread_mutex_t *mutex;
	
	worker_opt_t *opt = (worker_opt_t*) worker_optp;

	// loop
	while(1)
	{

		// Get appropriate mutex lock to use
		if(opt->appopt->lockmode == LOCK_FINE)
			mutex = &opt->handleopt->requests_mutex;
		else // lockmode == LOCK_COARSE
			mutex = &opt->handleopt->coarse_mutex;

		pthread_mutex_lock(mutex);

		// Wait for a request to process
		empty = ll_empty(opt->handleopt->requests);
		while(empty > 0 && !opt->handleopt->stop)
		{
			pthread_cond_wait(&opt->handleopt->requests_onnew, mutex);
			empty = ll_empty(opt->handleopt->requests);
		}

		// Stop if queue is empty and we were told to stop
		if(opt->handleopt->stop && empty > 0)
		{
			pthread_mutex_unlock(mutex);
			break;
		}

		// Pop a request from the queue if possible
		request = (request_t*) ll_pop_front(opt->handleopt->requests);

		// Fine lock no longer needed
		if(opt->appopt->lockmode == LOCK_FINE)
			pthread_mutex_unlock(mutex);

		if(request == NULL)
		{
			fprintf(stderr, "Worker got invalid request to process.\n");
			continue;
		}

		// Process request, pass of work to appropriate handler
		if(request->type == REQUEST_CHECK)
			result = request_check_process(request, opt->appopt);
		else if(request->type == REQUEST_TRANSACTION)
			result = request_transaction_process(request, opt->appopt);
		else
			// error
			result = REQUEST_RESULT_FAIL;

		// If request did not succeed and can be retried, do it again
		if(result == REQUEST_RESULT_RETRY)
		{
			//fprintf(stderr, "RETRY %d\n", request->id);

			// Wait a bit before retrying

			usleep(1);

			ll_push_back(opt->handleopt->requests, request);

			// Try to lock mutex during cond signal, but don't care if lock fails
			//pthread_mutex_lock(mutex);

			//pthread_cond_signal(&opt->handleopt->requests_onnew);

			//pthread_mutex_unlock(mutex);
		}
		else if(result == REQUEST_RESULT_FAIL)
		{
			// TODO
			// DEBUG
			fprintf(stderr, "Request %d failed.\n", request->id);
		} else if(result <= 0) {
			//TODO
			// DEBUG
			fprintf(stderr, "Request %d went wrong with status %d\n", request->id, result);
		}

		// Finished

		// Unlock coarse mutex
		if(opt->appopt->lockmode == LOCK_COARSE)
			pthread_mutex_unlock(mutex);

	}

	// Cleanup thread-specific things
	free(worker_optp);

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
	request->data = NULL;
	
	// Assume the request completes, it is non-critical
	gettimeofday(&request->start, NULL);
	
	requestId++;
	return request;
}

pthread_t *
requests_create_workers(request_handle_opt_t *options)
{
	int i;

	// TODO allocate pthread space
	pthread_t *workers = malloc(sizeof(pthread_t) * options->appoptions->workers);

	if(workers == NULL)
		return NULL;

	for(i = 0; i < options->appoptions->workers; i++)
	{
		worker_opt_t *opt = malloc(sizeof(worker_opt_t));

		if(opt == NULL)
		{
			// TODO
		}

		// Fill opt
		opt->id = i;
		opt->appopt = options->appoptions;
		opt->handleopt = options;

		pthread_create(&workers[i], NULL, requests_worker, opt);
	}

	return workers;
}

void
requests_join_workers(pthread_t ** workers, request_handle_opt_t *options)
{
	pthread_mutex_t *mutex;
	pthread_t thread;
	int i;

	if(workers == NULL)
		return;

	// Get mutex lock to use
	if(options->appoptions->lockmode == LOCK_FINE)
		mutex = &options->requests_mutex;
	else // lockmode == LOCK_COARSE
		mutex = &options->coarse_mutex;

	// Kicker broadcast to allow all workers to exit nicely
	options->stop = 1;
	pthread_mutex_lock(mutex);
	pthread_cond_broadcast(&options->requests_onnew);
	pthread_mutex_unlock(mutex);

	thread = (*workers)[0];
	for(i=0; (*workers+i) != NULL; i++, thread=(*workers)[i])
	{
		pthread_join(thread, NULL);
	}

	free(*workers);
	*workers = NULL;
}

void
requests_handle(app_options_t *options)
{

	// Initialize variables
	char *line = NULL;
	size_t linesize = 0;
	ssize_t read = 0;

	char **tokens = NULL;
	size_t tokensSize = 0;
	size_t tokenCount = 0;

	int cont = 1;
	pthread_t *workers;

	// Options used throughout request handling
	request_handle_opt_t handler;
	handler.appoptions = options;
	handler.stop = 0;
	pthread_mutex_init(&handler.coarse_mutex, NULL);
	pthread_mutex_init(&handler.request_list_mutex, NULL);
	pthread_mutex_init(&handler.requests_mutex, NULL);
	pthread_cond_init(&handler.requests_onnew, NULL);

	// Create bank accounts
	if(accounts_initialize(options) == 0)
		server_abort(ERROR_MEMORY_ALLOC);

	// Build a request list
	if(options->lockmode == LOCK_FINE)
		handler.requests = ll_initialize(&handler.request_list_mutex);
	else // lockmode == LOCK_COARSE
		handler.requests = ll_initialize(NULL);
	if(handler.requests == NULL)
		server_abort(ERROR_MEMORY_ALLOC);

	// Spin-up worker threads
	if((workers = requests_create_workers(&handler)) == NULL)
		server_abort(ERROR_THREADING);

	usleep(10000);

	// Read-parse-eval loop
	while (cont > 0)//(read != -1 && cont > 0)
	{

		// Read
		if(options->in == stdin)
			printf("%s", options->prompt);
		read = getline(&line, &linesize, options->in);

		if(read != -1)
		{

			// Scan
			tokenCount = tokLine(line, read, &tokens, &tokensSize);

			// Parse & eval
			cont = requests_handle_tokens(tokens, tokenCount, &handler);

			// Cleanup
			clearTok(tokens, tokenCount);

		}
	}

	// Join all worker threads
	requests_join_workers(&workers, &handler);

	// Now force output to flush
	fflush(options->out);

	// cleanup
	free(line);
	freeTok(tokens, tokenCount);
	ll_free(handler.requests);

	pthread_mutex_destroy(&handler.coarse_mutex);
	pthread_mutex_destroy(&handler.requests_mutex);
	pthread_mutex_destroy(&handler.request_list_mutex);
	pthread_cond_destroy(&handler.requests_onnew);
	
}
