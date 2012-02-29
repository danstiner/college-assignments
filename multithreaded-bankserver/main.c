#include "main.h"

int appserver(int argc, char **argv, int defaultLockMode)
{
	int opt;
	app_options_t options = APP_OPTIONS_INITIALIZER;

	// Check special options
	while((opt = getopt(argc, argv, "p:")) != -1)
		switch (opt)
		{
		case 'p':
			options.prompt = optarg;
			break;
		case '?':
			if(isprint(optopt))
				fprintf(stderr, "Unknown option '-%c'.\n", optopt);
		default:
			server_abort(ERROR_BAD_OPTIONS);
		}

	// Remaining options should be of the form
	// appserver <#workers> <#accounts> <outputfile>
	if(argc - optind < 2)
		server_abort(ERROR_BAD_OPTIONS);

	sscanf(argv[optind+0], "%d", &options.workers);
	sscanf(argv[optind+1], "%d", &options.accounts);

	if(argc - optind > 2)
	{
		options.out = fopen(argv[optind+2], "w");

		if(options.out == NULL)
			server_abort(ERROR_BAD_OUTPUTFILE);
	}

	requests_handle(&options);

	if(options.out != stdout)
		fclose(options.out);

	return EXIT_SUCCESS;
}

