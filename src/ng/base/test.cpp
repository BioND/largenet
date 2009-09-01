/**
 * @file test.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "DirectedGraph.h"
#include "Edge.h"
#include "Node.h"
#include <iostream>
#include <boost/assert.hpp>

using namespace largenet;


int main()
{
	DirectedGraph g;
	for (unsigned int i = 0; i < 10; ++i)
	{
		node_id_t n = g.addNode();
		BOOST_ASSERT(n == i);
	}
	BOOST_ASSERT(g.numberOfNodes() == 10);
	g.addEdge(0, 2);
	g.addEdge(3, 4);
	g.addEdge(9, 1);
	g.addEdge(1, 9);
//	g.addEdge(1, 9);

	DirectedGraph::NodeIteratorRange nodes(g.nodes());
	for (DirectedGraph::NodeIterator n(nodes.first); n != nodes.second; ++n)
	{
		std::cout << "k_in = " << n->second->inDegree() << ", k_out = " << n->second->outDegree() << "\n";
	}

	DirectedGraph::EdgeIteratorRange edges(g.edges());
	for (DirectedGraph::EdgeIterator e(edges.first); e != edges.second; ++e)
	{
		std::cout << e->second->source()->id() << " " << e->second->target()->id() << "\n";
	}

	return 0;
}

