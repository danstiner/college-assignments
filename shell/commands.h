#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "constants.h"
#include "errors.h"
#include "builtins.h"
#include "background.h"

void commands_init();

char *command_exit_status(int status);

void handle_tokens(char **tokens, size_t tokenCount, char *originalLine);

void handle_exec(char **tokens, size_t tokenCount, char *originalLine, bool background, int stdin_fd, int stdout_fd, int stderr_fd);

pid_t exec_command(char *command, char **arguments, int stdin_fd, int stdout_fd, int stderr_fd);


#endif
