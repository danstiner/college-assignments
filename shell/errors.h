#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR_UNKNOWN_OPTION 1
#define ERROR_MEMORY_ALLOC 31

#define EXIT_COMMAND_NOTFOUND 255

void resh_abort(char *message, int code);

#endif
