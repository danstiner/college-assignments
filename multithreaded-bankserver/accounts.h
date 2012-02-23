#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include "pthread.h"

struct account {
	pthread_mutex_t lock;
	int value;
};

#endif
