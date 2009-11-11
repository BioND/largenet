/**
 * @file test.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "largenet.h"
#include "StateConsistencyListener.h"

#include <iostream>
#include <fstream>
#include <boost/assert.hpp>
#include <stdexcept>

#include "../myrng1.2/myrngWELL.h"

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
		std::cout << "Added edge (" << e->source()->id() << ","
				<< e->target()->id() << ") in state " << g->edgeState(e->id())
				<< "\n";
	}
};

struct EdgeStateCalc
{
	edge_state_t operator()(const node_state_t s, const node_state_t t)
	{
		if (s != t)
			return 1;
		else if (s == 0)
			return 0;
		else
			return 2;
	}
};

int main()
{
	Graph g(2, 3);
	g.setElementFactory(std::auto_ptr<ElementFactory>(
			new MultiDirectedElementFactory));
	TestListener gl;
	StateConsistencyListener<EdgeStateCalc> scl(std::auto_ptr<EdgeStateCalc>(
			new EdgeStateCalc));
	g.addGraphListener(&scl).addGraphListener(&gl);
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
		std::cout << "Edge " << *e << ": " << e->source()->id() << " "
				<< e->target()->id() << "\n";
	}

	std::cout << "3-4 is ";
	if (!g.isEdge(3, 4))
		std::cout << "not ";
	std::cout << "an edge.\n";
	std::cout << "4-3 is ";
	if (!g.isEdge(4, 3))
		std::cout << "not ";
	std::cout << "an edge.\n";

	try
	{
		Node* nd = g.randomNode(rng);
		std::cout << "Node " << nd->id() << " is in state " << g.nodeState(
				nd->id()) << "\n";
		nd = g.randomNode(1, rng);
		std::cout << "Random node in state 1 has ID " << nd->id() << "\n";
		Node* nd2 = nd->randomOutNeighbor(rng);
		std::cout << "Random out neighbor of node " << nd->id() << " has ID "
				<< nd2->id() << "\n";
		Node* nd3 = nd->randomInNeighbor(rng);
		std::cout << "Random in neighbor of node " << nd->id() << " has ID "
				<< nd3->id() << "\n";
	} catch (const std::invalid_argument& e)
	{
		std::cerr << "\033[22;31m" << e.what() << "\n\033[0;0m";
	}
	std::auto_ptr<Edge> de(new Edge(10, *g.node(3), *g.node(4))), ue(
			new UndirectedEdge(11, *g.node(3), *g.node(4)));
	std::cout << "Edge(10, 3, 4) is" << (is_directed(*de) ? "" : " not")
			<< " directed.\n";
	std::cout << "UndirectedEdge(10, 3, 4) is" << (is_directed(*ue) ? ""
			: " not") << " directed.\n";



	Graph g2(1, 1);
	g2.setElementFactory(std::auto_ptr<ElementFactory>(
			new MultiDirectedElementFactory));
	erdosRenyi(g2, 5000, 0.001);
	std::ofstream f("test.edgelist");
	if (!f)
		return 1;
	writeEdgeList(g2, f);
	f.close();

	return 0;
}

