/*
 * pairingheap.h
 *
 *  Created on: Feb 15, 2012
 *      Author: stiner
 */

#ifndef PAIRINGHEAP_H_
#define PAIRINGHEAP_H_

#include <stdlib.h>
#include <list>



template<typename T>
class PairingHeap
{

private:

	class Heap
	{
	public:

		Heap(T *value);

		std::list<Heap*> subheaps;

		T *value;
	};

	Heap *root;

	Heap* MergePairs(std::list<Heap*> &heaps);

	Heap *Merge(Heap *heap1, Heap *heap2);

	void PrependSubheap(Heap *heap, Heap *to);

	void DeleteHeap(Heap *heap);

public:
	PairingHeap();

	~PairingHeap();

	T *
	FindMin();

	T *
	ExtractMin();

	void
	Insert(T *item);

};


template<class ItemType>
void PairingHeap<ItemType>::DeleteHeap(Heap *heap)
{
	typename std::list<Heap*>::iterator it;

	if (heap == NULL)
		return;

	// Recursively delete sub-heaps
	for (it = heap->subheaps.begin(); it != heap->subheaps.end(); it++)
	{
		DeleteHeap(*it);
	}

	delete heap;
}

template<class ItemType>
PairingHeap<ItemType>::~PairingHeap()
{
	DeleteHeap(this->root);
}

template<typename ItemType>
typename PairingHeap<ItemType>::Heap*
PairingHeap<ItemType>::MergePairs(std::list<Heap*> &heaps)
{
	if (heaps.size() == 0)
		return NULL;
	else if (heaps.size() == 1)
	{
		return heaps.pop_front();
	}
	else
	{
		Heap *heap1 = heaps.pop_front();
		Heap *heap2 = heaps.pop_front();
		return Merge(Merge(heap1, heap2), MergePairs(heaps));
	}
}

template<typename T>
typename PairingHeap<T>::Heap *
PairingHeap<T>::Merge(Heap *heap1, Heap *heap2)
{
	if (heap2 == NULL)
		return heap1;
	else if (heap1 == NULL)
		return heap2;
	else if (*heap1->value < *heap2->value)
	{
		heap1->subheaps.push_front(heap2);

		return heap1;
	}
	else
	{
		heap2->subheaps.push_front(heap1);

		return heap2;
	}
}


#endif /* PAIRINGHEAP_H_ */

