#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "errors.h"
#include "tokenizer.h"
#include "commands.h"


void readevalprint_loop(FILE *stream, char *prompt);

int main(int argc, char **argv) {
	int opt;
	char * prompt = DEFAULT_PROMPT_STRING;


	// TODO, parse command line arguments using getopt
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
			resh_abort("Unknown option given, exiting to minimize possible damage.", ERROR_UNKNOWN_OPTION);
		}

	commands_init();

	readevalprint_loop(stdin, prompt);

}


void readevalprint_loop(FILE *stream, char *prompt)
{
	char *line = NULL;
	char *linedup = NULL;
	size_t linesize = 0;
	ssize_t read = 0;

	char **tokens = NULL;
	size_t tokensSize = 0;
	size_t tokenCount;

	// Read
	while (read != -1)
	{

		// Read
		printf("%s", prompt);
		read = getline(&line, &linesize, stream);
		linedup = strdup(line);		
		linedup[read-1] = '\0';

		// Parse
		tokenCount = tokLine(line, read, &tokens, &tokensSize);

		// Eval
		handle_tokens(tokens, tokenCount, linedup);

		// Cleanup
		clearTok(tokens, tokenCount);
		free(linedup);
	}

	// cleanup
	free(line);
	freeTok(tokens, tokenCount);
}
