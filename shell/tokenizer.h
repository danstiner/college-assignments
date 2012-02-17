#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"

size_t tokLine(char *line, ssize_t linesize, char ***tokens, size_t *tokenCount);

int size2DArrayOfStrings(char ***array, size_t* size, size_t amount);

void freeTok(char **tokens, size_t count);

#endif
