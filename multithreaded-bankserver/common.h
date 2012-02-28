#ifndef COMMON_H
#define COMMMON_H

#include <sys/time.h>

struct timeval elapsed_time(struct timeval *since, struct timezone *tz);

#endif
