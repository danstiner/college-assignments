#include "linkedlist.h"

ll_t *ll_initialize()
{
	ll_t * list = (ll_t *) malloc(sizeof(ll_t));

	if(list == NULL)
		return NULL;

	list->prev = list;
	list->next = list;
	list->value = NULL;

	return list;
}

void ll_cull(ll_t *list_head, ll_cull_callback_func callback)
{
	ll_t *item = ll_get_next(list_head);

	while(item != list_head)
	{
		int result = callback(item->value);

		if(result != 0)
		{
			ll_t * temp = item->next;
			ll_remove(item);
			item = temp;
		}
		else
			item = item->next;
	}
}

ll_t *ll_find(ll_t *list_head, ll_find_callback_func callback, void *param)
{
	ll_t *item = ll_get_next(list_head);

	while(item != list_head)
	{
		int result = callback(item->value, param);

		if(result != 0)
			return item;
		else
			item = item->next;
	}
}

void ll_map(ll_t *list_head, ll_map_callback_func callback)
{
	ll_t *item = ll_get_next(list_head);

	while(item != list_head)
	{
		callback(item->value);
		item = item->next;
	}
}

int ll_set(ll_t *list_item, void *value)
{
	list_item->value = value;
	return 1;
}

void *ll_access(ll_t *list_item)
{
	return list_item->value;
}

int ll_remove(ll_t *list_item)
{
	list_item->prev->next = list_item->next;
	list_item->next->prev = list_item->prev;

	free(list_item);

	return 1;
}

ll_t *ll_get_next(ll_t *list_item)
{
	return list_item->next;
}

ll_t *ll_get_prev(ll_t *list_item)
{
	return list_item->prev;
}

int ll_insert(void *value, ll_t *after_item)
{
	ll_t *new_item = malloc(sizeof(ll_t));

	if(new_item == NULL)
		return 0;

	new_item->next = after_item->next;
	new_item->prev = after_item;
	new_item->value = value;

	after_item->next->prev = new_item;
	after_item->next = new_item;

	return 1;
}
