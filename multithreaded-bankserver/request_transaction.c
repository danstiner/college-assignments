#include "request_transaction.h"

int
request_transaction_is(char **tokens, int count)
{
	return count > 0 && strcmp("TRANS", tokens[0]) == 0;
}

int
request_transaction_isr(request_t *request)
{
	return request != NULL && request->type == REQUEST_TRANSACTION;
}

int
request_transaction_process(request_t *request, app_options_t *options)
{
	request_transaction_t *transaction;

	// Check that the request is a check
	if(!request_transaction_isr(request))
		return -1;
	
	transaction = (request_transaction_t*) request->data;

	if(transaction == NULL)
		return -1;

	return request_transaction_process_internal(transaction, request, options);
}

int
request_transaction_process_internal(request_transaction_t *request, request_t *prequest, app_options_t *options)
{
	int j;
	int i;
	int result = 1;
	int insufficient_funds = 0;
	struct timeval elapsed;
	locked_account_t accounts[request->count];
	int new_balances[request->count];

	// Get write locks right away
	for(i=0; i<request->count && result > 0; i++)
	{
		result = account_writelock(request->parts[i].account, request->parts[i].account_clock, &accounts[i]);
	}

	// If its a temporary failure, retry
	if(result == -1)
	{
		for(j=0; j<i; j++)
		{
			account_unlock(&accounts[i]);
		}
		return REQUEST_RESULT_RETRY;
	}

	// Check balances before commiting
	if(result > 0)
	{
		for(i=0; i<request->count && result > 0; i++)
		{
			result = account_readbalance(&accounts[i], &new_balances[i]);
		}
	}

	// Check amount to change account by is valid
	if(result > 0)
	{
		for(i=0; i<request->count && result > 0; i++)
		{
			int balance = new_balances[i];
			int amount = request->parts[i].amount;

			if(balance + amount < 0)
			{
				// insufficient funds, stop transaction
				insufficient_funds = request->parts[i].account;
				result = 0;
			}
			else
			{
				new_balances[i] = balance + amount;
			}
		}
	}

	// Attempt to update account balances
	if(result > 0)
	{
		for(i=0; i<request->count && result > 0; i++)
		{
			int balance = -1;

			// Change
			result = account_modifybalance(&accounts[i], request->parts[i].amount);

			if(result <= 0)
			{
				fprintf(stderr, "Unknown error %d while modifying balance of account %d in transaction %d\n", result, request->parts[i].account, prequest->id);
			}


			// Verify
			result = account_readbalance(&accounts[i], &balance);

			if(result <= 0)
			{
				fprintf(stderr, "Unknown error %d while reading balance of account %d in transaction %d\n", result, request->parts[i].account, prequest->id);
			}

			// Check new amount is correct
			if(balance != new_balances[i])
			{
				fprintf(stderr, "Balance not consistent after transaction for account %d in transaction %d\n", request->parts[i].account, prequest->id);
			}
		}
	}

	// Either a failure
	if(result <= 0 && insufficient_funds == 0)
	{
		// Unknown error, abort
		// so unlock accounts and return error
		for(i=0; i<request->count && result > 0; i++)
		{
			account_unlock(&accounts[i]);
		}

		return result;
	}
	// or success/ISF, which commits writes and unlocks
	else
	{
		// Commit writes and unlock
		for(i=0; i<request->count && result > 0; i++)
		{
			result = account_commit_writeinc(&accounts[i]);

			if(result <= 0)
			{
				// Too late to easily fix it now
				fprintf(stderr, "Unknown error %d while commiting changes to account %d in transaction %d\n", result, request->parts[i].account, prequest->id);
			} else {
				account_unlock(&accounts[i]);
			}

		}

		elapsed = elapsed_time(&prequest->start, NULL);

		if(insufficient_funds)
		{
			// Known error, insufficient funds
			fprintf(options->out, "%d ISF %d TIME %ld.%06ld\n", prequest->id, request->parts[i].account, elapsed.tv_sec, elapsed.tv_usec);
		}
		else
		{
			// Normal transaction
			fprintf(options->out, "%d OK TIME %ld.%06ld\n", prequest->id, elapsed.tv_sec, elapsed.tv_usec);
		}

		return 1;
	}
}

int
request_transaction_create(char **tokens, int count, request_t *request, app_options_t *options)
{
	int i;
	request_transaction_t *trans;

	if(count < 2 || (count-1)%2 != 0)
	{
		fprintf(stderr, "Usage: TRANS <acc1> <amount1> <acct2> <amount2> ...\n");
		return 0;
	}

	// Build a transaction request
	trans = request_transaction_new();

	if(trans == NULL)
	{
		fprintf(stderr, "Unabled to build TRANS.\n");
		return 0;
	}

	for(i = 1; i < count-1; i += 2)
	{
		int acct = -1;
		int amount = 0;
		int result = 0;

		sscanf(tokens[i], "%d", &acct);
		sscanf(tokens[i+1], "%d", &amount);

		result = request_transaction_add(trans, acct, amount, options);

		if(result <= 0)
		{
			fprintf(stderr, "Unable to create TRANS request.\n");
			request_transaction_destory(&trans);
			return 0;
		}
	}

	request->type = REQUEST_TRANSACTION;

	request->data = trans;

	return 1;
}

int
request_transaction_add(request_transaction_t *transaction, int account, int amount, app_options_t *options)
{
	void *tmp;
	request_transaction_part_t *part;

	if(account <= 0 || account > options->accounts)
	{
		fprintf(stderr, "Invalid trans account id: %d\n", account);
		return -1;
	}

	// Expand parts array to add ours
	tmp = realloc(transaction->parts, sizeof(request_transaction_part_t) * (transaction->count+1));

	if(tmp == NULL)
		return 0;

	transaction->parts = (request_transaction_part_t*) tmp;
	part = &transaction->parts[transaction->count];
	transaction->count++;

	part->account = account;

	// Store current account clock version, then postincrement
	account_request_writeinc(account, &part->account_clock);

	part->amount = amount;

	// Done.
	return 1;
}

int
request_transaction_destory(request_transaction_t **transactionp)
{
	if(transactionp == NULL || *transactionp == NULL)
		return 2;

	free((*transactionp)->parts);

	free(*transactionp);

	// Done.
	return 1;
}

request_transaction_t *
request_transaction_new()
{
	request_transaction_t *request = (request_transaction_t*) malloc(sizeof(request_transaction_t));

	// TODO
	request->parts = NULL;
	request->count = 0;

	return request;
}
