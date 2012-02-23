//============================================================================
// Name        : primslist.cpp
// Author      : Daniel Stiner <daniel.stiner@gmail.com>
//============================================================================

#include "Graph.hpp"

#include "common.hpp"

using namespace danielstiner;

int main(int argc, char **argv)
{
	minspanningtree::check_inputs(argc, argv);

	ifstream in(argv[1], ifstream::in);

	edge_list *edges = Graph::ReadEdgeList(in);

	adjacency_list *adj = Graph::BuildAdjacencyList(edges);

	spanning_tree mst = Graph::PrimsList(adj);

	minspanningtree::print_mst(argv[2], mst);

	return EXIT_SUCCESS;
}
