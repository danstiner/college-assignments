/*
 * pairingheap.c
 *
 *  Created on: Feb 15, 2012
 *      Author: stiner
 */

#include "pairingheap.h"


//
//template<class ItemType>
//PairingHeap<ItemType>::PairingHeap(ItemType *value)
//{
//	root = new Heap(value);
//}



template<class ItemType>
PairingHeap<ItemType>::PairingHeap()
{
	root = NULL;
}

template<class ItemType>
ItemType *
PairingHeap<ItemType>::ExtractMin()
{
	if (root == NULL)
	{ // error out
	}
	else
	{
//		Heap *h = MergePairs(root->subheaps);
//
//		delete root;
//
//		root = h;
	}
}

template<class ItemType>
ItemType *
PairingHeap<ItemType>::FindMin()
{
	if (this->root == NULL)
	{
		// Handle empty tree
		// error
	}
	else
	{
		return this->root->value;
	}
}

template<class ItemType>
void PairingHeap<ItemType>::PrependSubheap(PairingHeap::Heap *heap,
		PairingHeap::Heap *to)
{
	if (heap == NULL || to == NULL)
		return;

	to->subheaps.push_front(heap);

}

template<class ItemType>
void PairingHeap<ItemType>::Insert(ItemType *item)
{
	this->root = Merge(new Heap(item), root);
}

