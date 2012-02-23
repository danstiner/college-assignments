#include "errors.h"

void server_abort(int code)
{
	switch (code)
	{
	case ERROR_BAD_OPTIONS:
		fprintf(stderr, "Unknown option given on command line, exiting to minimize possible damage.\n");

		fprintf(stderr, "Proper Usage:\n");
		fprintf(stderr, "./appserver <#workers> <#accounts> <output_file>\n");
		break;
	case ERROR_MEMORY_ALLOC:
		fprintf(stderr, "Unable to allocate sufficient memory, exiting.\n");
		break;
	case ERROR_BAD_OUTPUTFILE:
		fprintf(stderr, "Unable to open output file for writing, exiting.\n");
		break;
	case ERROR_THREADING:
		fprintf(stderr, "Some kind of threading error occured, exiting.\n");
		break;
	default:
		fprintf(stderr, "Unknown error, exiting.\n");
	}
	printf("Error: %d\n", code);

	// Any cleanup should already be handled by caller

	exit(code);
}

