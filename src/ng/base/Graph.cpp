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
	elf_(std::auto_ptr<ElementFactory>(new SingleEdgeElementFactory<>))
{
}

Graph::~Graph()
{
}

void Graph::clear()
{
	edges_.clear();
	nodes_.clear();
}

node_id_t Graph::addNode()
{
	node_id_t id = nodes_.size();
	nodes_.insert(id, elf_->createNode(id));
	return id;
}

edge_id_t Graph::addEdge(const node_id_t source, const node_id_t target)
{
	edge_id_t id = edges_.size();
	edges_.insert(id, elf_->createEdge(id, node(source), node(target)));
	return id;
}

void Graph::removeNode(const node_id_t n)
{
	NodeContainer::iterator it(nodes_.find(n));
	if (it != nodes_.end())
		nodes_.erase(it);
}

void Graph::removeEdge(const edge_id_t e)
{
	EdgeContainer::iterator it(edges_.find(e));
	if (it != edges_.end())
		edges_.erase(it);
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
