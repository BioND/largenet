/**
 * @file DirectedGraph.cpp
 * @date 12.08.2009
 * @author gerd
 */

#include "DirectedGraph.h"
#include "Node.h"
#include "Edge.h"
#include "SingleEdgeElementFactory.h"

namespace largenet
{

DirectedGraph::DirectedGraph() :
	elf_(std::auto_ptr<ElementFactory>(new SingleEdgeElementFactory))
{
}

DirectedGraph::~DirectedGraph()
{
}

void DirectedGraph::clear()
{
	edges_.clear();
	nodes_.clear();
}

node_id_t DirectedGraph::addNode()
{
	node_id_t id = nodes_.size();
	nodes_.insert(id, elf_->createNode(id));
	return id;
}

edge_id_t DirectedGraph::addEdge(const node_id_t source, const node_id_t target)
{
	edge_id_t id = edges_.size();
	edges_.insert(id, elf_->createEdge(id, node(source), node(target)));
	return id;
}

void DirectedGraph::removeNode(const node_id_t n)
{
	NodeContainer::iterator it(nodes_.find(n));
	if (it != nodes_.end())
		nodes_.erase(it);
}

void DirectedGraph::removeEdge(const edge_id_t e)
{
	EdgeContainer::iterator it(edges_.find(e));
	if (it != edges_.end())
		edges_.erase(it);
}

DirectedGraph::NodeIteratorRange DirectedGraph::nodes()
{
	return std::make_pair(NodeIterator(nodes_.begin()), NodeIterator(
			nodes_.end()));
}

DirectedGraph::EdgeIteratorRange DirectedGraph::edges()
{
	return std::make_pair(EdgeIterator(edges_.begin()), EdgeIterator(
			edges_.end()));
}

}
