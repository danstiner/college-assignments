#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include "pthread.h"

struct account {
	/* Used for thread-safe reading or writing to account */
	pthread_rwlock_t lock;

	/** Account number */
	int id;

	/** id of the last request created to write to this account */
	int request_clock;
	/** id of the last request which wrote to this account */
	int process_clock;

	/**  */	
	pthread_mutex_t readm;

	/** Broadcast to when a read or write is completed on this account */
	pthread_cond_t read;
};

#endif
