#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR_BAD_OPTIONS 1
#define ERROR_MEMORY_ALLOC 2
#define ERROR_BAD_OUTPUTFILE 3
#define ERROR_THREADING 4

void server_abort(int code);

#endif
