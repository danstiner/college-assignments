//============================================================================
// Name        : appserver.c
// Author      : Daniel Stiner <daniel.stiner@gmail.com>
//============================================================================

#include "main.h"

int main(int argc, char **argv)
{
	return appserver(argc, argv, LOCK_FINE);
}
