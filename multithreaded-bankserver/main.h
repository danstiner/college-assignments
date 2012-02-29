#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct app_option_struct
{
	int lockmode;
	char * prompt;
	int accounts;
	int workers;
	FILE *in;
	FILE *out;
} app_options_t;

#include "errors.h"
#include "requests.h"

#define APP_OPTIONS_INITIALIZER (app_options_t) { LOCK_FINE, "> ", 0, 0, stdin, stdout }

int appserver(int argc, char **argv, int defaultLockMode);

#endif
