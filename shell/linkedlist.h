#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

// Doubly-linked circular list implementation

typedef void (*ll_map_callback_func)(void *);

typedef int (*ll_cull_callback_func)(void *);

typedef int (*ll_find_callback_func)(void *, void *);

typedef struct linkedlist_item {
	struct linkedlist_item *prev;
	struct linkedlist_item *next;
	void *value;
} ll_t;


ll_t *ll_initialize();
/** Easy list traveral
 * For each item in the list, callback is
 * invoked with the value pointer from that item
 */
void ll_map(ll_t *start_item, ll_map_callback_func callback);

ll_t *ll_find(ll_t *start_item, ll_find_callback_func callback, void *param);

void ll_cull(ll_t *list_head, ll_cull_callback_func callback);

int ll_set(ll_t *list_item, void *value);
void *ll_access(ll_t *list_item);
int ll_remove(ll_t *list_item);
ll_t *ll_get_next(ll_t *list_item);
ll_t *ll_get_prev(ll_t *list_item);
int ll_insert(void *value, ll_t *after_item);


#endif
