/**
 * @file DirectedGraph.cpp
 * @date 12.08.2009
 * @author gerd
 */

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "SingleEdgeElementFactory.h"

namespace largenet
{

Graph::Graph() :
	elf_(std::auto_ptr<ElementFactory>(new SingleEdgeElementFactory<> ))
{
}

Graph::~Graph()
{
	for (EdgeContainer::iterator i = edges_.begin(); i != edges_.end(); ++i)
		delete *i;
	for (NodeContainer::iterator i = nodes_.begin(); i != nodes_.end(); ++i)
		delete *i;
}

void Graph::clear()
{
	edges_.clear();
	nodes_.clear();
}

node_id_t Graph::addNode()
{
	// FIXME this is an ugly hack
	node_id_t id = nodes_.insert(0);
	nodes_[id] = elf_->createNode(id);
	return id;
}

edge_id_t Graph::addEdge(const node_id_t source, const node_id_t target)
{
	// FIXME this is an ugly hack
	edge_id_t id = edges_.insert(0);
	edges_[id] = elf_->createEdge(id, node(source), node(target));
	return id;
}

void Graph::removeNode(const node_id_t n)
{
	nodes_.erase(n);
}

void Graph::removeEdge(const edge_id_t e)
{
	edges_.erase(e);
}

Graph::NodeIteratorRange Graph::nodes()
{
	return std::make_pair(NodeIterator(nodes_.begin()), NodeIterator(
			nodes_.end()));
}

Graph::EdgeIteratorRange Graph::edges()
{
	return std::make_pair(EdgeIterator(edges_.begin()), EdgeIterator(
			edges_.end()));
}

}
