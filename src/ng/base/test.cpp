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
#include <fstream>
#include <boost/assert.hpp>

#include "../../myrng1.2/myrngWELL.h"

using namespace largenet;

void erdosRenyi(Graph& g, node_size_t nodes, double p)
{
	g.clear();
	for (node_size_t i = 0; i < nodes; ++i)
		g.addNode();
	Graph::NodeIteratorRange iters = g.nodes();
	for (Graph::NodeIterator i = iters.first; i != iters.second; ++i)
	{
		for (Graph::NodeIterator j = iters.first; j != iters.second; ++j)
		{
			if (rng.Chance(p))
				g.addEdge(*i, *j);
		}
	}
}

void writeEdgeList(Graph& g, std::ostream& o)
{
	Graph::EdgeIteratorRange edges = g.edges();
	for (Graph::EdgeIterator i = edges.first; i != edges.second; ++i)
		o << i->source()->id() << " " << i->target()->id() << "\n";
}

class TestListener: public GraphListener
{
	void afterNodeAddEvent(Graph* g, Node* n)
	{
		std::cout << "Added node " << n->id() << " in state " << g->nodeState(
				n->id()) << "\n";
	}
	void afterEdgeAddEvent(Graph* g, Edge* e)
	{
		std::cout << "Added edge (" << e->source()->id() << "," << e->target()->id() << ")\n";
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

	Node* nd = g.randomNode(rng);
	if (nd != 0)
		std::cout << "Node " << nd->id() << " is in state " << g.nodeState(nd->id()) << "\n";
	nd = g.randomNode(1, rng);
	if (nd != 0)
		std::cout << "Random node in state 1 has ID " << nd->id() << "\n";


	Graph g2(1, 1);
	g2.setElementFactory(std::auto_ptr<ElementFactory> (new MultiEdgeElementFactory<DirectedEdge>));
	erdosRenyi(g2, 5000, 0.001);
	std::ofstream f("test.edgelist");
	if (!f)
		return 1;
	writeEdgeList(g, f);
	f.close();

	return 0;
}

