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

	std::list<T> items;
public:
	ListHeap() {};
	~ListHeap() {};

	T
	ExtractMin();

	void
	Insert(T item);

};

template<class ItemType>
void ListHeap<ItemType>::Insert(ItemType item)
{
	items.push_front(item);
}

template<typename T>
T
ListHeap<T>::ExtractMin()
{
	T min;

//	if(items.size() == 0)
//		return NULL;

	min = items.front();

	typename std::list<T>::iterator it;

	for(it = items.begin(); it != items.end(); ++it)
	{
		if((*it) < min)
			min = *it;
	}

	items.remove(min);

	return min;
}


} /* namespace danielstiner */
#endif /* PAIRINGHEAP_H_ */
