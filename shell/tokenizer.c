#include "tokenizer.h"

size_t tokLine(char *line, ssize_t linesize, char *** tokens, size_t *tokenCount)
{
	size_t tokensRead = 0;
	char * tokp;

	// Drop terminating newline
	line[linesize-1] = '\0';
	linesize--;

	tokp = strtok (line," ");
	while (tokp != NULL)
	{
		// DEBUG
		//printf ("Token: %s\n",tokp);

		// Increment tokens read and expand tokens memory as needed
		tokensRead = tokensRead + 1;
		if(*tokenCount <= tokensRead) {
			size2DArrayOfStrings(tokens, tokenCount, tokensRead);
		}

		// Store a copy of read token
		(*tokens)[tokensRead-1] = strdup(tokp);

		// Parse next token
		tokp = strtok(NULL, " ");
	}

	// Shrink token array down to needed size
	//size2DArrayOfStrings(tokens, tokenCount, tokensRead);

	// Done parsing, tokens is now an array of malloc'd char strings
	return tokensRead;
}

int size2DArrayOfStrings(char ***array, size_t* size, size_t newSize)
{
	int i;

	if(0 == *size)
	{
		// DEBUG
		//printf("first alloc\n");
		*array = (char **) malloc(newSize * sizeof(char **));
		*size = newSize;
	}
	else if(*size != newSize)
	{
		if(newSize < *size)
			for(i=*size-1; i>newSize; i--)
			{
			//fprintf(stderr, "DEBUG: Freeing %d\n", i);
				if((*array)[i] != NULL)
				{
					free((*array)[i]);
					(*array)[i] = NULL;
				}
			}
		// DEBUG
		//printf("realloc\n");
		if(NULL == (*array = realloc(*array, newSize * sizeof(char **))))
		{
			resh_abort("Could not allocate memory", ERROR_MEMORY_ALLOC);
		}
		else
		{
			*size = newSize;
		}
	}
	return 0;
}

void clearTok(char **tokens, size_t count)
{
	size_t i;
	for(i = 0; i < count; i++)
	{
		if(tokens[i] != NULL)
		{
			// DEBUG
			//printf("Freeing %d\n", i);
			free(tokens[i]);
			tokens[i] = NULL;
		}
	}
}

void freeTok(char **tokens, size_t count)
{
	clearTok(tokens, count);
	free(tokens);
}
