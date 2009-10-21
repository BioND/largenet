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

Graph::Graph(const node_state_t nodeStates, const edge_state_t edgeStates) :
	elf_(std::auto_ptr<ElementFactory>(new SingleEdgeElementFactory<> )),
			nodes_(nodeStates), edges_(edgeStates)
{
}

Graph::~Graph()
{
	for (EdgeContainer::iterator i = edges_.begin(); i != edges_.end(); ++i)
		delete *i;
	for (NodeContainer::iterator i = nodes_.begin(); i != nodes_.end(); ++i)
		delete *i;
}

bool Graph::isDirected() const
{
	return elf_->directedEdges();
}

void Graph::clear()
{
	beforeGraphClear();
	edges_.clear();
	nodes_.clear();
}

node_id_t Graph::addNode()
{
	return addNode(0);
}

node_id_t Graph::addNode(const node_state_t s)
{
	// FIXME this is an ugly hack
	node_id_t id = nodes_.insert(0, s);
	nodes_[id] = elf_->createNode(id);
	afterNodeAdd(id);
	return id;
}

edge_id_t Graph::addEdge(const node_id_t source, const node_id_t target)
{
	edge_id_t id = edges_.insert(0);
	Edge* e = 0;
	try
	{
		// FIXME this is an ugly hack
		e = elf_->createEdge(id, node(source), node(target));
		edges_[id] = e;
		afterEdgeAdd(id);
		return id;
	} catch (SingletonException& ex)
	{
		// edge exists and we do not allow multiple edges
		edges_.erase(id);
		delete e;
		if (isDirected())
			return node(source)->edgeTo(node(target))->id();
		else
			return node(source)->edgeToAdjacentNode(node(target))->id();
	}
}

void Graph::removeNode(const node_id_t n)
{
	beforeNodeRemove(n);
	nodes_.erase(n);
}

void Graph::removeEdge(const edge_id_t e)
{
	beforeEdgeRemove(e);
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

bool Graph::isEdge(const node_id_t source, const node_id_t target) const
{
	assert(nodes_.valid(source));
	assert(nodes_.valid(target));
	if (isDirected()) // FIXME
		return node(source)->hasEdgeTo(node(target));
	else
		return node(source)->isAdjacentTo(node(target));
}

void Graph::afterNodeAdd(const node_id_t n)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterNodeAdd(this, node(n));
}

void Graph::afterEdgeAdd(const edge_id_t e)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterEdgeAdd(this, edge(e));
}

void Graph::beforeNodeRemove(const node_id_t n)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->beforeNodeRemove(this, node(n));
}

void Graph::beforeEdgeRemove(const edge_id_t e)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->beforeEdgeRemove(this, edge(e));
}

void Graph::beforeGraphClear()
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->beforeGraphClear(this);
}

void Graph::afterNodeStateChange(const node_id_t n,
		const node_state_t oldState, const node_state_t newState)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterNodeStateChange(this, node(n), oldState, newState);
}

void Graph::afterEdgeStateChange(const edge_id_t e,
		const edge_state_t oldState, const edge_state_t newState)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterEdgeStateChange(this, edge(e), oldState, newState);
}

}
