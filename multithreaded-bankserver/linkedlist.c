#include "linkedlist.h"

ll_t *ll_initialize(pthread_mutex_t *mutex)
{
	ll_t *list = (ll_t *) malloc(sizeof(ll_t));

	if(list == NULL)
		return NULL;

	list->tail = NULL;
	list->head = NULL;
	list->mutex = mutex;

	return list;
}

int ll_free(ll_t *list)
{
	pthread_mutex_t *mutex;
	ll_item_t *itemp;
	ll_item_t *itemdp;

	// We need a mutex to protect list while deleting it
	if(list == NULL)
		return 0;

	mutex = list->mutex;

	if(mutex != NULL && pthread_mutex_lock(mutex) != 0)
		return 0;

	if(list->head != NULL)
	{
		// Actually delete list items
		itemp = list->head;
		while (itemp != list->tail)
		{
			itemp = itemp->next;
			itemdp = itemp;
			free(itemdp);
		}
		// also delete the tail itself
		free(list->tail);
	}

	free(list);

	if(mutex != NULL && pthread_mutex_unlock(mutex) != 0)
		return -1;
	else
		return 1; // Success
}

void *ll_peek_back(ll_t *list)
{
	if(list == NULL || list->tail == NULL)
		return NULL;

	return list->tail->value;
}

void *ll_pop_back(ll_t *list)
{
	pthread_mutex_t *mutex;
	void *value;

	if(list == NULL || list->tail == NULL)
		return NULL;

	mutex = list->mutex;

	// We need a mutex to protect list while removing
	if(mutex != NULL && pthread_mutex_lock(mutex) != 0)
		return NULL;

	if(list == NULL || list->tail == NULL)
	{
		if(mutex != NULL)
			pthread_mutex_unlock(mutex);
		return NULL;
	}

	value = list->tail->value;

	if(list->tail == list->head)
	{
		// Only one item on the list, remove it
		
		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		// Unlink list->tail from the list
		ll_item_t *tmp = list->tail;
		list->tail = list->tail->prev;
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		free(tmp);
	}

	if(mutex != NULL && pthread_mutex_unlock(mutex) != 0)
		return NULL;
	else
		return value;
}

void *ll_pop_front(ll_t *list)
{
	pthread_mutex_t *mutex;
	void *value;

	if(list == NULL)
		return NULL;

	mutex = list->mutex;

	// We need a mutex to protect list while removing
	if(mutex != NULL && pthread_mutex_lock(mutex) != 0)
		return NULL;

	if(list == NULL || list->head == NULL)
	{
		if(mutex != NULL)
			pthread_mutex_unlock(mutex);
		return NULL;
	}

	value = list->head->value;

	if(list->tail == list->head)
	{
		// Only one item on the list, remove it
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		// Unlink list->tail from the list
		ll_item_t *tmp = list->head;
		list->head = list->head->next;
		list->head->prev = NULL;
		//tmp->prev->next = tmp->next;
		//tmp->next->prev = tmp->prev;
		free(tmp);
	}

	if(mutex != NULL && pthread_mutex_unlock(mutex) != 0)
		return NULL;
	else
		return value;
}

int ll_push_back(ll_t *list, void *value)
{
	pthread_mutex_t *mutex;
	ll_item_t *new_item;

	if(list == NULL)
		return 0;

	if(NULL == (new_item = malloc(sizeof(ll_item_t))))
		return 0;

	mutex = list->mutex;
	
	// Now we need a mutex to protect list while adding to it
	if(mutex != NULL)
		pthread_mutex_lock(mutex);

	if(list == NULL)
	{
		free(new_item);
		if(mutex != NULL)
			pthread_mutex_unlock(mutex);
		return 0;
	}

	new_item->value = value;

	if(list->tail == NULL)
	{
		new_item->next = NULL;
		new_item->prev = NULL;
		list->tail = new_item;
		list->head = new_item;
	}
	else
	{
		//new_item->next = list->tail->next;
		new_item->prev = list->tail;

		//list->tail->next->prev = new_item;
		list->tail->next = new_item;

		list->tail = new_item;
	}

	if(mutex != NULL)
		pthread_mutex_unlock(mutex);
	
	return 1;
}


int
ll_empty(ll_t *list)
{
	pthread_mutex_t *mutex;
	int result = 0;

	if(list == NULL)
		return 2;

	mutex = list->mutex;

	// We need a mutex to protect list while removing
	if(mutex != NULL)
		pthread_mutex_lock(mutex);

	if(list == NULL)
		result = 2;

	if(list->head == NULL)
		result = 1;

	if(mutex != NULL)
		pthread_mutex_unlock(mutex);
	
	return result;
}
