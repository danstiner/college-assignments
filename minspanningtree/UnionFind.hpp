/*
 * UnionFind.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: Daniel Stiner
 */

#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include <boost/unordered/unordered_map.hpp>

namespace danielstiner
{

using namespace boost::unordered;

template<typename T>
class UnionFind
{

private:

	class UnionSet
	{

	public:

		int size;

		UnionSet *parent;

		bool isRootSet;

		UnionSet();

	};

	typedef unordered_map<const T, UnionFind<T>::UnionSet*> set_map_t;

	set_map_t map;

	UnionSet*
	Find(const T &element);

	UnionSet*
	InsertInternal(const T &element);

public:

	UnionFind();
	~UnionFind();

	void
	Insert(const T &element);

	void
	Union(const T &a, const T &b);

	bool
	InSameSet(const T &a, const T &b);

};

template<typename T>
inline UnionFind<T>::UnionSet::UnionSet()
{
	this->size = 1;
	this->parent = this;
	this->isRootSet = true;
}

template<typename T>
inline UnionFind<T>::UnionFind()
{
}

template<typename T>
inline UnionFind<T>::~UnionFind()
{
	typename set_map_t::iterator it;

	for (it = map.begin(); it != map.end(); ++it)
	{
		delete it->second;
	}
}

template<typename T>
inline typename UnionFind<T>::UnionSet *UnionFind<T>::InsertInternal(const T &element)
{
	UnionSet * tmp = new UnionSet();
	this->map[element] = tmp;
	return tmp;
}

template<typename T>
inline void UnionFind<T>::Insert(const T &element)
{
	this->map[element] = new UnionSet();
}

template<typename T>
inline void UnionFind<T>::Union(const T &element1, const T &element2)
{
	UnionSet *a = Find(element1);
	UnionSet *b = Find(element2);

	if (a->size < b->size)
	{
		UnionSet *tmp = b;
		b = a;
		a = tmp;
	}
	b->parent = a;
	b->isRootSet = false;

	a->size += b->size;

}

template<typename T>
inline bool UnionFind<T>::InSameSet(const T &element1, const T &element2)
{
	return (Find(element1) == Find(element2));
}

template<typename T>
inline typename UnionFind<T>::UnionSet *UnionFind<T>::Find(const T &element)
{
	UnionSet *root_set = NULL;

	try
	{
		root_set = map.at(element);
	} catch (std::out_of_range &e)
	{
		root_set = InsertInternal(element);
	}

	if (!root_set->isRootSet)
	{
		root_set = root_set->parent;
	}

	// if the direct parent is not the root set
	// then find the root and update all parent pointer
	// to equal the root
	if (!root_set->isRootSet)
	{
		// Finds root set always
		while (!root_set->isRootSet)
			root_set = root_set->parent;

		// Update parent references
		UnionSet *set = map.at(element);
		while (!set->isRootSet)
		{
			UnionSet *tmp = set;
			set = set->parent;
			tmp->parent = root_set;
		}

	}

	return root_set;
}

}

#endif /* PAIRINGHEAP_H_ */
