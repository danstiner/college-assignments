#include "request_check.h"

int
request_check_is(char **tokens, int count)
{
	return count > 0 && strcmp("CHECK", tokens[0]) == 0;
}

int
request_check_isr(request_t *request)
{
	return request != NULL && request->type == REQUEST_CHECK;
}

request_check_t *
request_check_new()
{
	request_check_t *request = (request_check_t*) malloc(sizeof(request_check_t));

	request->account = -1;
	request->account_clock = -1;

	return request;
}

int
request_check_process(request_t *request, app_options_t *options)
{
	int result = -1;
	int balance = -1;
	locked_account_t account;
	request_check_t *check;
	struct timeval elapsed;

	// Check that the request is a check
	if(!request_check_isr(request))
		return -1;


	check = (request_check_t *)request->data;

	if(check == NULL)
		return -1;

	// Get lock on account
	result = account_readlock(check->account, check->account_clock, &account);

	// negative one means temporarily unable to get lock
	if(result == -1)
	{
		return REQUEST_RESULT_RETRY;
	}

	if(result <= 0)
	{
		return result;
	}

	result = account_readbalance(&account, &balance);

	if(result <= 0)
	{
		account_unlock(&account);

		return result;
	}

	result = account_commit_readinc(&account);

	if(result <= 0)
	{
		account_unlock(&account);

		return result;
	}

	gettimeofday(&elapsed, NULL);

	// Print out result
	fprintf(options->out, "%d BAL %d TIME %ld.%06ld %ld.%06ld\n",
		request->id,
		balance,
		request->start.tv_sec,
		request->start.tv_usec,
		elapsed.tv_sec,
		elapsed.tv_usec);

	fflush(options->out);

	return 1;
}

int
request_check_create(char **tokens, int count, request_t *request, app_options_t *options)
{
		int acct;
		request_check_t *data;

		if(count < 2)
		{
			fprintf(stderr, "Usage: CHECK <accountid>\n");
			return 0;
		}
		sscanf(tokens[1], "%d", &acct);

		if(acct <= 0 || acct > options->accounts)
		{
			fprintf(stderr, "Invalid check account id: %d\n", acct);
			return 0;
		}

		// build check request
		data = request_check_new(acct);

		if(data == NULL)
		{
			fprintf(stderr, "Unable to create CHECK request.\n");
			return 0;
		}

		data->account = acct;

		// Store current account clock version, then postincrement
		account_request_readinc(acct, &data->account_clock);

		request->data = (void*)data;

		request->type = REQUEST_CHECK;

		// Done
		return 1;
}
