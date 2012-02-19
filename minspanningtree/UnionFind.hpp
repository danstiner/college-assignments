/*
 * UnionFind.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: Daniel Stiner
 */

#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include <stdlib.h>
#include <list>

namespace danielstiner
{

template<typename T>
class UnionFind
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

template<typename T>
PairingHeap<T>::Heap::Heap(T *value)
{
	this->value = value;
}

template<typename T>
PairingHeap<T>::PairingHeap()
{
	root = NULL;
}

template<typename T>
PairingHeap<T>::~PairingHeap()
{
	// TODO Auto-generated destructor stub
}

template<class ItemType>
ItemType *
PairingHeap<ItemType>::FindMin()
{
	if (this->root == NULL)
	{
		// Handle empty tree
		// error
		return NULL;
	}
	else
	{
		return this->root->value;
	}
}

template<class ItemType>
void PairingHeap<ItemType>::Insert(ItemType *item)
{
	this->root = Merge(new Heap(item), root);
}

template<class ItemType>
ItemType *
PairingHeap<ItemType>::ExtractMin()
{
	if (root == NULL)
	{ // error out
		return NULL;
	}
	else
	{
		ItemType *ret = root->value;
		Heap *h = MergePairs(root->subheaps);

		delete root;

		root = h;

		return ret;
	}
}

template<typename ItemType>
typename PairingHeap<ItemType>::Heap*
PairingHeap<ItemType>::MergePairs(std::list<Heap*> &heaps)
{
	if (heaps.size() == 0)
		return NULL;
	else if (heaps.size() == 1)
	{
		Heap *heap = heaps.front();
		heaps.pop_front();
		return heap;
	}
	else
	{
		Heap *heap1 = heaps.front();
		heaps.pop_front();
		Heap *heap2 = heaps.front();
		heaps.pop_front();
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

} /* namespace danielstiner */
#endif /* PAIRINGHEAP_H_ */
