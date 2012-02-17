#include "builtins.h"

int try_handle_builtin(char **argv, int argc, FILE *out)
{
        // Built-in must have at least one argument, its name
        if(argc < 1)
                return FALSE;

        if(strcmp("exit", argv[0]) == 0)
        {
                // EXIT
                // TODO cleanup
                exit(0);
        }
        else if(strcmp("pid", argv[0]) == 0)
        {
                // PID
                fprintf(out, "My pid is: %d\n", getpid());
        }
        else if(strcmp("ppid", argv[0]) == 0)
        {
                // PPID
                fprintf(out, "My parent's pid is: %d\n", getppid());
        }
        else if(strcmp("cd", argv[0]) == 0)
        {
                // CD <dir>
		cd_builtin(argv, argc, out);
        }
        else if(strcmp("pwd", argv[0]) == 0)
        {
                // PWD
                pwd_builtin(argv, argc, out);
        }
	else if(strcmp("get", argv[0]) == 0)
	{
		// GET <var>
		get_builtin(argv, argc, out);
	}
        else if(strcmp("set", argv[0]) == 0)
        {
                // SET <var> <value>
		// SET <var>
		set_builtin(argv, argc, out);
        }
        else if(strcmp("jobs", argv[0]) == 0)
        {
                // JOBS
		jobs_builtin(argv, argc, out);
        }
	else if(strcmp(":", argv[0]) == 0)
	{
	}
	else
        {
                return FALSE;
        }

        return TRUE;
}

void mv_builtin(char **argv, int argc, FILE *out)
{
	int result;

	if(argc <= 3)
	{
		fprintf(stderr, "Missing source or destination filename.");
		fprintf(stderr, "mv <source> <destination>");
		return;
	}
	
	result = rename(argv[1], argv[2]);
	
	if(result != 0)
		perror("resh");
}

void jobs_builtin(char **argv, int argc, FILE *out)
{
	background_print_status(out);
}

void get_builtin(char **argv, int argc, FILE *out)
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage: get <varname>\n");
	}
	else
	{
		char *value = getenv(argv[1]);
		
		if(value == NULL)
			fprintf(stderr, "Environment varible '%s' not found.\n", argv[1]);
		else
			fprintf(out, "%s\n", value);
	}
}

void set_builtin(char **argv, int argc, FILE *out)
{
	if(argc <= 1)
	{
		// Print all envornment values
		// They should be in char **environ, which is null terminated
		char **envsetting = environ;
		
		do {
			printf("%s\n", *envsetting);
		} while(*(++envsetting) != NULL);
	}
	else if(argc == 2)
	{
		// Clear setting
		unsetenv(argv[1]);
	}
	else
	{
		// Set setting
		setenv(argv[1], argv[2], 1);
	}
}

void cd_builtin(char **argv, int argc, FILE *out)
{
	int result;
	char *path;

	if(argc == 1)
	{
		// No path given, assume cd $HOME
		path = getenv("HOME");
	}
	else
	{
		// Path given as second argument
		path = argv[1];
	}
	

	result = chdir(path);
	
	if(result != 0)
		fprintf(stderr, "Failed to change directories.\n");

	if(errno == EACCES)
		fprintf(stderr, "Search permission denied in part of path: %s\n", path);
	else if(errno == EFAULT)
		fprintf(stderr, "Fault accessing path, not a valid part of address space");
	else if(errno == EIO)
		printf("I/O error occurred, sorry.");
	else if(errno == ELOOP)
	{
		fprintf(stderr, "Encountered many symbolic loops while resolving path : %s\n", path);
		fprintf(stderr, "Please do not attempt further path-ception.\n");
	}
	else if(errno == ENAMETOOLONG)
		printf("Too long of a path name: %s\n", path);
	else if(errno == ENOMEM)
		printf("Out of kernel memory, prepare ye-self for a system crash.\n");
	else if(errno == ENOTDIR)
		printf("No tricks please, not all parts of the given path are directories");

}
void pwd_builtin(char **argv, int argc, FILE *out) {
	char *buffer = getcwd(NULL, 0);
	
	if(buffer == NULL)
	{
		fprintf(stderr, "Unable to allocate sufficient memory for patha");
		return;
	}

	fprintf(out, "%s\n", buffer);
	
	free(buffer);
}
