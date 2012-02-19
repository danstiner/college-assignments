//============================================================================
// Name        : test-PairingHeap.cpp
// Author      : Daniel Stiner <daniel.stiner@gmail.com>
//============================================================================

#include <iostream>
#include <fstream>
#include <map>
#include <list>

#include "PairingHeap.hpp"


#include "ListHeap.hpp"

class Node
{

};

template<typename T>
class Edge
{
public:
	T fromNodeId;
	T toNodeId;
	float weight;

	Edge() {}
	Edge(const Edge& copy) : fromNodeId(copy.fromNodeId), toNodeId(copy.toNodeId), weight(copy.weight) {}

	inline bool operator<(const Edge<T> &other)
	{
		return weight < other.weight;
	}

	inline bool operator==(const Edge<T> &other)
	{
		if(weight == other.weight
		&& fromNodeId == other.fromNodeId
		&& toNodeId == other.toNodeId)
			return true;
		else
			return false;
	}
};

template<typename T>
std::istream& operator>>(std::istream& is, Edge<T>& e)
{
	is >> e.fromNodeId;
	is >> e.toNodeId;
	is >> e.weight;

	return is;
}

template<typename T>
std::ostream& operator<<(std::ostream& is, Edge<T>& e)
{
	is << e.fromNodeId << " ";
	is << e.toNodeId << " ";
	is << e.weight << std::endl;

	return is;
}

//template<typename T>
//std::istream& operator>> (std::istream& is, Edge* str );

using namespace danielstiner;
using namespace std;

map<int, list<Edge<int> > > readInputToAdjacencyLists(istream &input);
Edge<int> findLightestEdge(ListHeap<Edge<int> > &heap,
		const map<int, bool> &closedSet);

int main(int argc, char **argv)
{
//	istream in(cin);

//	if(argc > 1)
//{
		ifstream fin(argv[1], ifstream::in);
//in = fin;
//}

	map<int, list<Edge<int> > > adjacency = readInputToAdjacencyLists(fin);

	map<int, bool> closedSet;

	list<Edge<int> > mst;

	ListHeap<Edge<int> > heap;

	// Start with the from node of first edge in the adjacency lists
	int node = (*adjacency.begin()).second.front().fromNodeId;
	Edge<int> minEdge;
	closedSet[node] = true;

	list<Edge<int> >::iterator it;

	map<int, list<Edge<int> > >::iterator im;

//	for(im = adjacency.begin(); im != adjacency.end(); ++im)
//	{
//		for(it = im->second.begin(); it != im->second.end(); ++it)
//		{
//			cerr << *it << endl;
//		} 
//	}


	while (closedSet.size() < adjacency.size())
	{

		list<Edge<int> > adjacent = adjacency[node];

		//cerr << "Node:a " << node << endl;

		// Copy all outgoing edges of node from adjacency list to heap
		for (it = adjacent.begin(); it != adjacent.end(); ++it)
		{
			heap.Insert((*it));
		}

		//cerr << "Node:b " << node << ", " << closedSet.size() << endl;



		// Grab minimum edge not ending in the closed set
		minEdge = findLightestEdge(heap, closedSet);


		//if(NULL == minEdge)
		//	break;

		node = minEdge.toNodeId;

		//cerr << "Node: c" << node << endl;

		// Add this edge to the mst
		// and mark the to node as in the closed set
		mst.push_back(minEdge);
		closedSet[node] = true;

	}

	for (it = mst.begin(); it != mst.end(); ++it)
	{
		cout << (*it).fromNodeId << " " << (*it).toNodeId << endl;

if(it->toNodeId == 0)
{
			cerr << *it;
		cerr << "help!" << endl;
}
	}

// TODO

	return EXIT_SUCCESS;
}

Edge<int> findLightestEdge(ListHeap<Edge<int> > &heap,
		const map<int, bool> &closedSet)
{
	Edge<int> minEdge;
	// Grab minimum edge not ending in the closed set
	minEdge = heap.ExtractMin();

	//if(minEdge == NULL)
	//	return NULL;

		if(minEdge.toNodeId == 0)
{
			cerr << minEdge;
		cerr << "help1!" << endl;
}

	while (closedSet.count(minEdge.toNodeId) > 0)
	{
		minEdge = heap.ExtractMin();

		//if(minEdge == NULL)
		//	return NULL;
	}

	return minEdge;
}

map<int, list<Edge<int> > > readInputToAdjacencyLists(istream &input)
{
	int nodeCount;
	int edgeCount;

	// Read "<# of nodes>\n<# of edges>"
	input >> nodeCount;
	input >> edgeCount;

	map<int, list<Edge<int> > > adjacency;

	Edge<int> e;

	for (int i = 0; i < edgeCount; i++)
	{
		char line[100];
		input >> e;

		adjacency[e.fromNodeId].push_back(e);

		cout << adjacency[e.fromNodeId].back();
	}

	return adjacency;
}
