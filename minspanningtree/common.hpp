/*
 * common.hpp
 *
 *  Created on: Feb 20, 2012
 *      Author: stiner
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <iostream>
#include <fstream>

#include "Graph.hpp"

namespace danielstiner
{

namespace minspanningtree
{

using namespace std;

void check_inputs(int argc, char **argv)
{
	if(argc < 3)
	{
		cout << "Usage:" << endl;
		cout << "./" << argv[0] << " input_graph_file output_mst_file" << endl;

		cout << "Where the input file is of the form:" << endl;
		cout << "  <number of unique vertices>" << endl;
		cout << "  <number of edges>" << endl;
		cout << "  <vertex> <vertex> <weight>" << endl;
		cout << "  <vertex> <vertex> <weight>" << endl;
		cout << "  ..." << endl;

		exit(2);
	}
}

void print_mst(char *filename, spanning_tree mst)
{
	ofstream out(filename, ofstream::out);

	// Sum weight of mst and print
	float weight = 0.0;
	spanning_tree::iterator it;
	for (it = mst.begin(); it != mst.end(); ++it)
	{
		weight = weight + it->weight;
	}
	cout << "Minimum Spanning Tree Weight: " << weight << endl;

	// print out mst
	Graph::WriteEdges(mst, out);
}

}

}

#endif /* COMMON_HPP_ */
