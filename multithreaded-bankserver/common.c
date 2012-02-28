#include "common.h"

struct timeval
elapsed_time(struct timeval *since, struct timezone *tz)
{
	struct timeval elapsed;

	gettimeofday(&elapsed, tz);

	// Compute difference
	elapsed.tv_sec = elapsed.tv_sec - since->tv_sec;
	elapsed.tv_usec = elapsed.tv_usec - since->tv_usec;

	// Deal with possible negative usec
	if(elapsed.tv_usec < 0)
	{
		elapsed.tv_sec -= 1;
		elapsed.tv_usec += 1000000;
	}

	return elapsed;
}
