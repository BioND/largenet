/**
 * @file test.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "Node.h"
#include <iostream>

namespace lnet
{

struct DirectedGraph
{
	typedef unsigned long int vertex_descriptor;
	typedef unsigned long int edge_descriptor;
	typedef directed_tag directed_category;
	typedef disallow_parallel_edge_tag edge_parallel_category;

	typedef unsigned long int degree_size_type;
	typedef unsigned long int vertices_size_type;
	typedef unsigned long int edges_size_type;
};

struct UndirectedGraph
{
	typedef unsigned long int vertex_descriptor;
	typedef unsigned long int edge_descriptor;
	typedef undirected_tag directed_category;
	typedef disallow_parallel_edge_tag edge_parallel_category;

	typedef unsigned long int degree_size_type;
	typedef unsigned long int vertices_size_type;
	typedef unsigned long int edges_size_type;
};

typedef Node<directedS, DirectedGraph::edge_descriptor> DirectedNode;
typedef Node<undirectedS, UndirectedGraph::edge_descriptor> UndirectedNode;

void testNode()
{
	DirectedNode nd;
	UndirectedNode und;

	nd.addInEdge(0);
	nd.addInEdge(1);
	nd.addInEdge(2);
	nd.addOutEdge(3);
	std::cout << nd.degree();
	std::cout << nd.inDegree();

	und.addEdge(2);
	und.removeEdge(2);
	std::cout << und.degree();
}
}
