#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <pthread.h>

// Doubly-linked list implementation

typedef struct linkedlist_item {
	struct linkedlist_item *prev;
	struct linkedlist_item *next;
	void *value;
} ll_item_t;

typedef struct linkedlist {
	ll_item_t *head;
	ll_item_t *tail;
	pthread_mutex_t *mutex;
} ll_t;


ll_t *ll_initialize(pthread_mutex_t *mutex);

void *ll_pop_front(ll_t *list);
void *ll_pop_back(ll_t *list);
int ll_push_back(ll_t *list, void *value);

int ll_empty(ll_t *list);

int ll_free(ll_t *list);

#endif
