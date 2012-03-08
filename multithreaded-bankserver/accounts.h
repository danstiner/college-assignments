#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <pthread.h>

#include "main.h"
#include "Bank.h"


#define ACCOUNT_LOCK_TOOSOON -1
#define ACCOUNT_LOCK_FAIL 0

typedef struct account_lock_struct
{
	int id;

	int state;
} locked_account_t;


int accounts_initialize(app_options_t *options);

int account_request_readinc(int account, int* clock);

int account_request_writeinc(int account, int* clock);

int account_readlock(int account, int at_clock, locked_account_t *account_lock);

int account_readbalance(locked_account_t *account, int *balance);

int account_commit_readinc(locked_account_t *account);

int account_writelock(int account, int at_clock, locked_account_t *account_lock);

int account_modifybalance(locked_account_t *account, int amount);

int account_commit_writeinc(locked_account_t *account);

int account_unlock(locked_account_t* lock);


// private

typedef struct account_struct {
	/* Used for thread-safe reading or writing to account */
	pthread_rwlock_t lock;

	/** Account number */
	int id;

	int request_read_clock;

	/** id of the last request created to write to this account */
	int request_write_clock;

	
	int processed_read_clock;

	/** id of the last request which wrote to this account */
	int processed_write_clock;

	/**  */	
	pthread_mutex_t mutex;

	/** Broadcast to when a read or write is completed on this account */
	pthread_cond_t read;

} account_t;


#endif
