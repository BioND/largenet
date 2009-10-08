/**
 * @file test.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "Graph.h"
#include "Edge.h"
#include "Node.h"
#include "MultiEdgeElementFactory.h"
#include "SingleEdgeElementFactory.h"

#include "GraphListener.h"

#include <iostream>
#include <boost/assert.hpp>

using namespace largenet;

class TestListener: public GraphListener
{
	void afterNodeAddEvent(Graph* g, Node* n)
	{
		std::cout << "Added node " << n->id() << " in state " << g->nodeState(
				n->id()) << "\n";
	}
};

int main()
{
	Graph g(2, 3);
	g.setElementFactory(std::auto_ptr<ElementFactory>(
			new MultiEdgeElementFactory<DirectedEdge> ));
	TestListener gl;
	g.addGraphListener(&gl);
	for (unsigned int i = 0; i < 10; ++i)
		g.addNode(i % 2);
	BOOST_ASSERT(g.numberOfNodes() == 10);
	g.addEdge(0, 2);
	g.addEdge(3, 4);
	g.addEdge(9, 1);
	g.addEdge(1, 9);
	g.addEdge(1, 9); // only allowed with MultiGraphs

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

	std::cout << "3-4 is ";
	if (!g.isEdge(3, 4))
		std::cout << "not ";
	std::cout << "an edge.\n";
	std::cout << "4-3 is ";
	if (!g.isEdge(4, 3))
		std::cout << "not ";
	std::cout << "an edge.\n";

	return 0;
}

