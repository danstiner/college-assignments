/*
 * PairingHeap.h
 *
 *  Created on: Feb 15, 2012
 *      Author: stiner
 */

#ifndef LISTHEAP_H_
#define LISTHEAP_H_

#include <stdlib.h>
#include <list>

namespace danielstiner
{

template<typename T>
class ListHeap
{

private:

	std::list<T*> items;
public:
	ListHeap()
	{
	}
	;
	~ListHeap()
	{
	}
	;

	T*
	ExtractMin();

	void
	Insert(T *item);

};

template<class T>
void ListHeap<T>::Insert(T *item)
{
	items.push_front(item);
}

template<typename T>
T*
ListHeap<T>::ExtractMin()
{
	T* min;
	typename std::list<T*>::iterator it;

	if (items.size() == 0)
		return NULL;

	// Find min
	min = items.front();
	for (it = items.begin(); it != items.end(); ++it)
	{
		if (*(*it) < *min)
			min = *it;
	}

	// Remove min
	for (it = items.begin(); it != items.end(); ++it)
	{
		if (*it == min) {
			items.erase(it);
			break;
		}
	}

	return min;
}

} /* namespace danielstiner */
#endif /* PAIRINGHEAP_H_ */
