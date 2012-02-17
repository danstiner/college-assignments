#include "commands.h"

void commands_init()
{
	background_init();
}

void print_command_exit_status(int status, FILE *out)
{
	if(WIFEXITED(status))
	{
		int exit_status = WEXITSTATUS(status);
		fprintf(out, "Exit %d", exit_status);
	}
	else if(WIFSIGNALED(status))
	{
		int term_sig = WTERMSIG(status);
		fprintf(out, "Killed (%d)", term_sig);
	}
	else if(WIFSTOPPED(status))
	{
		int stop_sig = WSTOPSIG(status);
		fprintf(out, "Stopped (%d)", stop_sig);
	}
	else
	{
		fprintf(out, "Unknown exit status");
	}
}

pid_t exec_command(char *command, char **arguments, int stdin_fd, int stdout_fd, int stderr_fd)
{
	pid_t pid;

	pid = fork();

	if(pid == 0)
	{

		// Redirect our output to given fd's
		dup2(STDIN_FILENO, stdin_fd);
		dup2(stdout_fd, STDOUT_FILENO);
		dup2(stderr_fd, STDERR_FILENO);

		execvp(command, arguments);
	
		// handle bad command
		printf("Cannot find command: %s\n", command);
		exit(EXIT_COMMAND_NOTFOUND);
	}
	else
	{
		return pid;
	}
}

void handle_exec(char **tokens, size_t tokenCount, char *originalLine, bool background, int stdin_fd, int stdout_fd, int stderr_fd)
{
	int i;
	char *command = NULL;
	char **arguments = NULL;
	int child_status;
	int child_id;
	pid_t child_pid;

	// In the case of /bin/ls
	// command should be /bin/ls
	// but first argument should be ls
	command = strdup(tokens[0]);

	// Make a copy of tokens to use as arguments to exec
	arguments = malloc((tokenCount+1) * sizeof(char*));

	if(arguments == NULL)
		resh_abort("Insufficient memories", ERROR_MEMORY_ALLOC);

	// First argument is basename of program
	arguments[0] = basename(command);

	for(i=1; i<tokenCount; i++)
	{
		arguments[i] = strdup(tokens[i]);

		if(tokens[i] != NULL && arguments[i] == NULL)
			resh_abort("Insufficient memories", ERROR_MEMORY_ALLOC);
	}
	// end arguments with a null terminator
	arguments[tokenCount] = NULL;

	// DO EXEC
	child_pid = exec_command(command, arguments, stdin_fd, stdout_fd, stderr_fd);

	if(background == TRUE)
	{
		// save created child to jobs, named by the line the user typed in
		child_id = background_add_job(child_pid, originalLine);
		
		// print child's internal id and pid and name
		printf("[%d] %d %s\n", child_id, child_pid, arguments[0]);
	}
	else
	{
		// print child's pid
		printf("[%d] %s\n", child_pid, arguments[0]);

		// wait for child to exit
		waitpid(child_pid, &child_status, 0);

		// Print exit status
		fprintf(stdout, "[%d] %s ", child_pid, arguments[0]);
		print_command_exit_status(child_status, stdout);
		fprintf(stdout, "\n");
	}

	// Cleanup
	free(command);
	
	// First argument is a basename of command, so no need to free it
	for(i=1; i<tokenCount; i++)
	{
		if(arguments[i] != NULL)
			free(arguments[i]);
	}
	free(arguments);
}

void handle_tokens(char **tokens, size_t tokenCount, char *originalLine)
{
	int background;
	int stdin_fd = STDIN_FILENO;
	int stdout_fd = STDOUT_FILENO;
	int stderr_fd = STDERR_FILENO;

	// DEBUG
	//int i;
	//printf ("Token Count: %i\n",tokenCount);
	//for(i=0; i<tokenCount; i++)
	//{
	//	fprintf (stderr, "Token: %s\n",tokens[i]);
	//}

	// If last token is "&", run in background
	if(tokenCount >= 2 && 0 == strcmp(tokens[tokenCount-1], "&"))
	{
		background = TRUE;
		tokenCount--;
	}
	else
		background = FALSE;

	// If second to last token is >, output to file
	if(tokenCount >= 3 && 0 == strcmp(tokens[tokenCount-2], ">"))
	{
		int fd = open(tokens[tokenCount-1], O_CREAT|O_TRUNC|O_WRONLY);

		// TODO check errno
		if(fd != -1)
			stdout_fd = fd;

		tokenCount--;
		tokenCount--;
	}


	// check token count is at least 1
	// i.e. ignore empty input line
	// and See if this is a built-in command
	if(tokenCount >= 1
	&& !try_handle_builtin(tokens, tokenCount, stdout))
	{
		handle_exec(tokens, tokenCount, originalLine, background, stdin_fd, stdout_fd, stderr_fd);
	}
	
	// Check on any background processes
	background_check_and_print();
}
