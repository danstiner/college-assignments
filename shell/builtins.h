#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <errno.h>

#include "constants.h"
#include "background.h"

extern char **environ;

int try_handle_builtin(char **argv, int argc, FILE *out);

void pwd_builtin(char **argv, int argc, FILE *out);
void get_builtin(char **argv, int argc, FILE *out);
void set_builtin(char **argv, int argc, FILE *out);
void cd_builtin(char **argv, int argc, FILE *out);
void mv_builtin(char **argv, int argc, FILE *out);
void jobs_builtin(char **argv, int argc, FILE *out);


#endif
