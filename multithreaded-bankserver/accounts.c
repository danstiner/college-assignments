#include "accounts.h"


account_t *accounts;

int g_account_count;


int account_isvalid(int account)
{
	return account >= 0 && account < g_account_count;
}

int accounts_parse(int account)
{
	return account - 1;
}


int accounts_initialize(app_options_t *options)
{
	int i;
	if(initialize_accounts(options->accounts) == 0)
		server_abort(ERROR_MEMORY_ALLOC);

	g_account_count = options->accounts;

	// accounts
	accounts = malloc(sizeof(account_t) * g_account_count);
	if(accounts == NULL)
		return 0;

	for(i=0; i<g_account_count; i++)
	{
		accounts[i].id = i;
		accounts[i].request_read_clock = 0;
		accounts[i].request_write_clock = 0;
		accounts[i].processed_read_clock = 0;
		accounts[i].processed_write_clock = 0;
		accounts[i].lock = PTHREAD_RWLOCK_INITIALIZER;
		accounts[i].lock = PTHREAD_RWLOCK_INITIALIZER;
		accounts[i].mutex = PTHREAD_MUTEX_INITIALIZER;
		accounts[i].read = PTHREAD_COND_INITIALIZER;
	}

	return 1;
}

int account_request_readinc(int account, int* clock)
{
	account = accounts_parse(account);
	if(!account_isvalid(account))
		return 0;

	*clock = accounts[account].request_write_clock;

	// DEBUG
	//fprintf(stderr, "Read request clock: %d\n", accounts[account].request_read_clock);

	// DEBUG
	//fprintf(stderr, "Write request clock: %d\n", accounts[account].request_write_clock);

	accounts[account].request_read_clock++;

	return 1;
}

int account_request_writeinc(int account, int* clock)
{
	account = accounts_parse(account);
	if(!account_isvalid(account))
		return 0;

	*clock = accounts[account].request_read_clock;

	// DEBUG
	//fprintf(stderr, "Read request clock: %d\n", accounts[account].request_read_clock);

	// DEBUG
	//fprintf(stderr, "Prev Write request clock: %d\n", accounts[account].request_write_clock);

	accounts[account].request_write_clock = ++accounts[account].request_read_clock;

	// DEBUG
	//fprintf(stderr, "Write request clock: %d\n", accounts[account].request_write_clock);

	return 1;
}

int account_readlock(int account, int at_clock, locked_account_t *account_lock)
{
	account = accounts_parse(account);
	if(!account_isvalid(account) || account_lock == NULL)
		return 0;

	// DEBUG
	//fprintf(stderr, "Read check clock: %d?=%d\n", at_clock, accounts[account].processed_write_clock);

	if(accounts[account].processed_write_clock != at_clock)
		return -1;

	// TODO get actual lock maybe?

	account_lock->id = account;

	return 1;
}

int account_readbalance(locked_account_t *account, int *balance)
{
	if(account == NULL || !account_isvalid(account->id))
		return 0;

	// TODO check lock status

	*balance = read_account( account->id + 1 );

	return 1;
}

int account_commit_readinc(locked_account_t *account)
{
	if(account == NULL || !account_isvalid(account->id))
		return 0;

	// TODO check lock status maybe/release

	accounts[account->id].processed_read_clock++;

	// DEBUG
	//fprintf(stderr, "Read inc clock: %d\n", accounts[account->id].processed_read_clock);

	// DEBUG
	//fprintf(stderr, "Write inc clock: %d\n", accounts[account->id].processed_write_clock);

	return 1;
}

int account_writelock(int account, int at_clock, locked_account_t *account_lock)
{
	account = accounts_parse(account);
	if(!account_isvalid(account) || account_lock == NULL)
		return 0;
	

	// DEBUG
	//fprintf(stderr, "Write check clock: %d?=%d\n", at_clock, accounts[account].processed_read_clock);

	if(accounts[account].processed_read_clock != at_clock)
		return -1;

	// TODO get actual lock maybe?

	account_lock->id = account;

	return 1;
}

int account_modifybalance(locked_account_t *account, int amount)
{
	if(account == NULL || !account_isvalid(account->id))
		return 0;

	// TODO check lock status

	write_account(account->id+1, amount + read_account(account->id+1) );

	// flip out
	if(read_account(account->id) < 0)
	{
		fprintf(stderr, "Bank account %d went below 0 balance, not good.\n", account->id);
		return -1;
	}

	return 1;
}

int account_commit_writeinc(locked_account_t *account)
{
	if(account == NULL || !account_isvalid(account->id))
		return 0;

	// TODO check lock status maybe/release

	accounts[account->id].processed_write_clock = ++accounts[account->id].processed_read_clock;

	// DEBUG
	//fprintf(stderr, "Read inc clock: %d\n", accounts[account->id].processed_read_clock);

	// DEBUG
	//fprintf(stderr, "Write inc clock: %d\n", accounts[account->id].processed_write_clock);

	return 1;
}

int account_unlock(locked_account_t* lock)
{
	// TODO

	return 1;
}
