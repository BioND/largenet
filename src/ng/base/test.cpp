/**
 * @file test.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "Graph.h"
#include "Edge.h"
#include "Node.h"
#include "MultiEdgeElementFactory.h"

#include "PropertyMap.h"
#include <iostream>
#include <boost/assert.hpp>

using namespace largenet;

int main()
{
	Graph g;
	g.setElementFactory(std::auto_ptr<ElementFactory>(new MultiEdgeElementFactory<UndirectedEdge>));
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
	g.addEdge(1, 9);	// only allowed with MultiGraphs

	Graph::NodeIteratorRange nodes(g.nodes());
	for (Graph::NodeIterator n(nodes.first); n != nodes.second; ++n)
	{
		std::cout << "Node " << *n << ": k_in = " << n->inDegree()
				<< ", k_out = " << n->outDegree() << "\n";
	}

	Graph::EdgeIteratorRange edges(g.edges());
	for (Graph::EdgeIterator e(edges.first); e != edges.second; ++e)
	{
		std::cout << e->source()->id() << " " << e->target()->id() << "\n";
	}

	PropertyMap<node_id_t, int> pm(3, g.numberOfNodes());
	pm[0] = 1;
	pm[1] = 2;

	return 0;
}

