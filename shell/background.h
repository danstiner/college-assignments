#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

#include "errors.h"
#include "constants.h"
#include "linkedlist.h"

typedef struct {
	int id;
	int status;
	int zombie;
	pid_t pid;
	char *name;
} background_job;

ll_t *g_background_jobs_list;

void background_init();

void background_print_status(FILE *out);

void background_check_and_print();

/** Stores the job information.
 * Makes a copy of the line
 * returns an internal id for the added job
 */
int background_add_job(int pid, char *name);

// private methods
void background_clear_job(ll_t *job_item);

void background_check_callback(void *value);
void background_printjob_callback(void *value);
void background_printjob(background_job *job);
int background_culljob_callback(void *value);
int background_findjob_callback(void *itemvalue, void *paramvalue);

#endif
