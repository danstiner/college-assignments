#include "errors.h"

void resh_abort(char *message, int code)
{
	printf("Error: %s", message);

	// TODO Any cleanup

	exit(code);
}

