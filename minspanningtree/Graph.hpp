/*
 * Graph.hpp
 *
 *  Created on: Feb 19, 2012
 *      Author: stiner
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "PairingHeap.hpp"
#include "ListHeap.hpp"
#include "UnionFind.hpp"

namespace danielstiner
{

using namespace boost;
using namespace std;

template<typename T>
class Node
{
public:
	T id;
};

template<typename T>
class Edge
{
public:
	T fromNodeId;
	T toNodeId;
	float weight;

	Edge()
	{
	}
	Edge(const Edge& copy) :
			fromNodeId(copy.fromNodeId), toNodeId(copy.toNodeId), weight(
					copy.weight)
	{
	}

	inline bool operator<(const Edge<T> &other)
	{
		return weight < other.weight;
	}

	inline bool operator==(const Edge<T> &other)
	{
		if (weight == other.weight && fromNodeId == other.fromNodeId
				&& toNodeId == other.toNodeId)
			return true;
		else
			return false;
	}
};

template<typename T>
inline std::istream& operator>>(std::istream& is, Edge<T>& e)
{
	is >> e.fromNodeId;
	is >> e.toNodeId;
	is >> e.weight;

	return is;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& is, Edge<T>& e)
{
	is << e.fromNodeId << " ";
	is << e.toNodeId << " ";
	is << e.weight;

	return is;
}

typedef boost::unordered_map<int, std::list<Edge<int>*> > adjacency_list;

typedef std::list<Edge<int>*> edge_list;

typedef std::list<Edge<int> > spanning_tree;

class Graph
{

private:
	static bool
	CompareEdgeP(Edge<int> *first, Edge<int> *second);

public:
	static adjacency_list *
	BuildAdjacencyList(edge_list *edges);

	static edge_list *
	ReadEdgeList(std::istream & input);

	static spanning_tree
	Prims(adjacency_list *graph);

	static spanning_tree
	PrimsList(adjacency_list *graph);

	static spanning_tree
	Kruskals(edge_list *edges);

	static
	void
	WriteEdges(list<Edge<int> > &edges, std::ostream &output);

};

void Graph::WriteEdges(spanning_tree &edges, std::ostream &output)
{
	spanning_tree::iterator it;

	unordered_set<int> vertices;

	// Count number of vertices
	for (it = edges.begin(); it != edges.end(); ++it)
	{
		vertices.insert(it->fromNodeId);
		vertices.insert(it->toNodeId);
	}

	// Output number of vertices and edges
	output << vertices.size() << endl;
	output << edges.size() << endl;

	for (it = edges.begin(); it != edges.end(); ++it)
		output << *it << endl;
}

spanning_tree Graph::Kruskals(edge_list *edges)
{
	list<Edge<int> > mst;

	edge_list::iterator it;

	UnionFind<int> unionfind;

	Edge<int> *edge;

	// Sort edges
	edges->sort(CompareEdgeP);

	// Look at each edge or until mst is full
	for (it = edges->begin(); it != edges->end(); ++it)
	{
		edge = *it;

		// Add it to the tree if it does not create a loop
		if (!unionfind.InSameSet(edge->fromNodeId, edge->toNodeId))
		{
			mst.push_back(*edge);
			unionfind.Union(edge->fromNodeId, edge->toNodeId);
		}
	}

	return mst;
}

inline bool Graph::CompareEdgeP(Edge<int> *first, Edge<int> *second)
{
	return (*first < *second);
}

list<Edge<int> > Graph::Prims(adjacency_list *graph)
{

	unordered_set<int> closedSet;

	list<Edge<int> > mst;

	PairingHeap<Edge<int> > heap;

	list<Edge<int>*>::iterator it;

	int node;

	Edge<int> *minEdge;
	bool toClosed;
	bool fromClosed;

	// Start with the from node of first edge in the adjacency lists
	node = graph->begin()->second.front()->fromNodeId;
	closedSet.insert(node);

	while (closedSet.size() < graph->size())
	{

		list<Edge<int>*> adjacent = graph->at(node);

		//cerr << "Node:a " << node << endl;

		// Copy all outgoing edges of node from adjacency list to heap
		for (it = adjacent.begin(); it != adjacent.end(); ++it)
		{
			if ((*it)->fromNodeId == node)
			{
				// Edge actual signifies edge (from, to)
				toClosed = (0 < closedSet.count((*it)->toNodeId));
				if (!toClosed)
					heap.Insert(*it);
			}
			else if ((*it)->toNodeId == node)
			{
				// Edge actual signifies edge (to, from)
				toClosed = (0 < closedSet.count((*it)->fromNodeId));
				if (!toClosed)
					heap.Insert(*it);
			}
			else
			{
				cerr << "ERROR: Weird stuff" << endl;

				return mst;
			}

			// Could implement decrement key on the heap to get slightly better runtime
			// but since we check edges as they are removed from the heap, its not really needed
			// Any edges between the same (u,v) with higher weights will be considered second
			// and thus ignored
		}

		// Grab minimum edge with exactly one edge in closed set
		do
		{
			minEdge = heap.ExtractMin();

			if (minEdge == NULL)
			{
				cerr << "ERROR: Ending on NULL edge" << endl;

				exit(1);
			}

			toClosed = (0 < closedSet.count(minEdge->toNodeId));
			fromClosed = (0 < closedSet.count(minEdge->fromNodeId));

		} while (!(toClosed ^ fromClosed));

		if (fromClosed)
			node = minEdge->toNodeId;
		else
			//toClosed
			node = minEdge->fromNodeId;

		// Add this edge to the mst
		// and mark the to node as in the closed set
		mst.push_back(*minEdge);
		closedSet.insert(node);

	}

	return mst;
}

list<Edge<int> > Graph::PrimsList(adjacency_list *graph)
{

	unordered_set<int> closedSet;

	list<Edge<int> > mst;

	ListHeap<Edge<int> > heap;

	list<Edge<int>*>::iterator it;

	int node;

	Edge<int> *minEdge;
	bool toClosed;
	bool fromClosed;

	// Start with the from node of first edge in the adjacency lists
	node = graph->begin()->second.front()->fromNodeId;
	closedSet.insert(node);

	while (closedSet.size() < graph->size())
	{

		list<Edge<int>*> adjacent = graph->at(node);

		//cerr << "Node:a " << node << endl;

		// Copy all outgoing edges of node from adjacency list to heap
		for (it = adjacent.begin(); it != adjacent.end(); ++it)
		{
			if ((*it)->fromNodeId == node)
			{
				// Edge actual signifies edge (from, to)
				toClosed = (0 < closedSet.count((*it)->toNodeId));
				if (!toClosed)
					heap.Insert(*it);
			}
			else if ((*it)->toNodeId == node)
			{
				// Edge actual signifies edge (to, from)
				toClosed = (0 < closedSet.count((*it)->fromNodeId));
				if (!toClosed)
					heap.Insert(*it);
			}
			else
			{
				cerr << "ERROR: Weird stuff" << endl;

				return mst;
			}

			// Could implement decrement key on the heap to get slightly better runtime
			// but since we check edges as they are removed from the heap, its not really needed
			// Any edges between the same (u,v) with higher weights will be considered second
			// and thus ignored
		}

		// Grab minimum edge with exactly one edge in closed set
		do
		{
			minEdge = heap.ExtractMin();

			if (minEdge == NULL)
			{
				cerr << "ERROR: Ending on NULL edge" << endl;

				exit(1);
			}

			toClosed = (0 < closedSet.count(minEdge->toNodeId));
			fromClosed = (0 < closedSet.count(minEdge->fromNodeId));

		} while (!(toClosed ^ fromClosed));

		if (fromClosed)
			node = minEdge->toNodeId;
		else
			//toClosed
			node = minEdge->fromNodeId;

		// Add this edge to the mst
		// and mark the to node as in the closed set
		mst.push_back(*minEdge);
		closedSet.insert(node);

	}

	return mst;
}

adjacency_list *
Graph::BuildAdjacencyList(edge_list *edges)
{
	edge_list::iterator it;
	adjacency_list *adjacency = new adjacency_list();

	// Iterate edges
	for (it = edges->begin(); it != edges->end(); ++it)
	{
		// Add edge to from and to adjacency list
		Edge<int> * edge = *it;

		// TODO: Use try-catch instead of this explicit check possibly
		if (adjacency->count(edge->fromNodeId) == 0)
			(*adjacency)[edge->fromNodeId] = list<Edge<int>*>();

		if (adjacency->count(edge->toNodeId) == 0)
			(*adjacency)[edge->toNodeId] = list<Edge<int>*>();

		adjacency->at(edge->fromNodeId).push_back(edge);
		adjacency->at(edge->toNodeId).push_back(edge);
	}

	return adjacency;
}

edge_list *Graph::ReadEdgeList(std::istream & input)
{
	int i;
	int nodeCount;
	int edgeCount;
	edge_list *edges;
	edge_list::iterator it;
	Edge<int> *edgearr;
	char buffer[16184];
	char line[80];

	// Speed up reading from file
	setlocale(LC_ALL, "C");
	input.rdbuf()->pubsetbuf(buffer, 16184);

	// Read "<# of nodes>\n<# of edges>"
	input >> nodeCount;
	input >> edgeCount;
	// Skip the extra \n
	input.getline(line, 80);

	// Pre-allocate space to store edges
	edges = new edge_list(edgeCount);
	edgearr = new Edge<int> [edgeCount];

	// Read edges of the form
	// <from_int> <to_int> <weight_float>
	// one per line
	i = 0;
	for (it = edges->begin(); it != edges->end(); ++it)
	{
		//Edge<int> *e = new Edge<int>();

		input.getline(line, 80);

		sscanf(line, "%d %d %f", &edgearr[i].fromNodeId, &edgearr[i].toNodeId,
				&edgearr[i].weight);

		//input >> edgearr[i];

		// Pointer magic
		*it = edgearr + i++;
	}

	return edges;
}

}

/* namespace danielstiner */
#endif /* GRAPH_H_ */
