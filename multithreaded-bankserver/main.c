#include "main.h"

int appserver(int argc, char **argv, int defaultLockMode)
{
	int opt;
	int workerCount;
	int accountCount;
	FILE *output;
	char * prompt = "> ";
	int lockMode = defaultLockMode;


	// Check special options
	while((opt = getopt(argc, argv, "p:")) != -1)
		switch (opt)
		{
		case 'p':
			prompt = optarg;
			break;
		case '?':
			if(isprint(optopt))
				fprintf(stderr, "Unknown option '-%c'.\n", optopt);
		default:
			server_abort(ERROR_BAD_OPTIONS);
		}

	// Remaining options should be of the form
	// appserver <#workers> <#accounts> <outputfile>
	if(argc-optind < 3)
		server_abort(ERROR_BAD_OPTIONS);

	sscanf(argv[optind+0], "%d", &workerCount);
	sscanf(argv[optind+1], "%d", &accountCount);
	output = fopen(argv[optind+2], "w");

	if(output == NULL)
		server_abort(ERROR_BAD_OUTPUTFILE);

	requests_handle(stdin, output, workerCount, accountCount, prompt, lockMode);

	return EXIT_SUCCESS;
}

