#include "background.h"


void background_init()
{
	background_job *empty_job = malloc(sizeof(background_job));
	if(empty_job == NULL)
		resh_abort("Memory Issues", ERROR_MEMORY_ALLOC);

	empty_job->id = -1;
	empty_job->pid = 0;
	empty_job->name = "EmptyJob";

	g_background_jobs_list = ll_initialize();
	ll_set(g_background_jobs_list, empty_job);
}

void background_print_status(FILE *out)
{
	// Print all background processes
	// if we have any ended processes, print them remove them
	// the next skips over our empty job

	// First update stored status of all background jobs
	ll_map(g_background_jobs_list, &background_check_callback);

	// Next print all stored statuses
	ll_map(g_background_jobs_list, &background_printjob_callback);

	// Next remove all stored but dead jobs
	ll_cull(g_background_jobs_list, &background_culljob_callback);
}

void background_check_and_print()
{
	pid_t child_pid;
	int status;
	ll_t *job_listitem;
	background_job job;
	background_job *jobp;

	// check for any child who has ended, but don't wait
	while((child_pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0)
	{
		// We have a finished background process

		// Look it up in the list of jobs

		// using a hacky iterator pattern here
		job.pid = child_pid;
		job_listitem = ll_find(g_background_jobs_list, &background_findjob_callback, &job);
		jobp = (background_job*) ll_access(job_listitem);
		jobp->status = status;

		if(WIFEXITED(status) || WIFSIGNALED(status))
		{
			jobp->zombie = 1;

			background_printjob(jobp);

			free(jobp);
			ll_remove(job_listitem);
		}
	}

	
}

// Check if first paramter refers to the same job as the second
int background_findjob_callback(void *itemvalue, void *paramvalue)
{
	background_job *itemjob = (background_job*) itemvalue;
	background_job *paramjob = (background_job*) paramvalue;

	if(itemjob->pid == paramjob->pid
	&& itemjob->pid > 0)
		return TRUE;
	else
		return FALSE;
}


// is-a ll_map_callback_func
void background_check_callback(void *value)
{
	background_job *job = (background_job*) value;

	job->zombie = waitpid(job->pid, &(job->status), WNOHANG|WUNTRACED|WCONTINUED);
}

void background_printjob_callback(void *value)
{
	background_job *job = (background_job*) value;

	background_printjob(job);
}

void background_printjob(background_job *job)
{
	char status[80];

 	if(job->zombie == 0)
		sprintf(status, "Running");
	else if(WIFEXITED(job->status))
                sprintf(status, "Exited %d", WEXITSTATUS(job->status));
        else if(WIFSIGNALED(job->status))
                sprintf(status, "Killed (%d)", WTERMSIG(job->status));
        else if(WIFSTOPPED(job->status))
                sprintf(status, "Stopped %d", WSTOPSIG(job->status));
        else if(WIFCONTINUED(job->status))
                sprintf(status, "Continued");
        else
		sprintf(status, "resh Error");

	fprintf(stdout, "[%d] %d %s \t%s\n", job->id, job->pid, status, job->name);

}

int background_culljob_callback(void *value)
{
	background_job *job = (background_job*) value;

	if(job->zombie != 0)
	{
		free(value);
		return TRUE;
	}

	return FALSE;
}

int background_add_job(int pid, char *name)
{
	background_job *latest;
	// Create a new job
	background_job *job = malloc(sizeof(background_job));
	
	job->zombie = 0;
	job->pid = pid;
	job->name = strdup(name);

	// Our id is one more than the newest background job
	latest = (background_job*) ll_access(ll_get_prev(g_background_jobs_list));
	job->id = 1 + latest->id;

	ll_insert(job, ll_get_prev(g_background_jobs_list));
	
	return job->id;
}


void background_clear_job(ll_t *job_item)
{
	// TODO
}
