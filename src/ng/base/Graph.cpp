/**
 * @file Graph.cpp
 * @date 12.08.2009
 * @author gerd
 */

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "factories.h"
#include "GraphListener.h"

namespace largenet
{

Graph::Graph(const node_state_t nodeStates, const edge_state_t edgeStates) :
	elf_(std::auto_ptr<ElementFactory>(new SingleUndirectedElementFactory)),
			nodes_(nodeStates), edges_(edgeStates)
{
}

Graph::~Graph()
{
//	for (EdgeContainer::iterator i = edges_.begin(); i != edges_.end(); ++i)
//		delete *i;
//	for (NodeContainer::iterator i = nodes_.begin(); i != nodes_.end(); ++i)
//		delete *i;
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

void Graph::setNodeState(const node_id_t n, const node_state_t s)
{
	const node_state_t old = nodeState(n);
	if (s == old)
		return;
	nodes_.setCategory(n, s);
	afterNodeStateChange(n, old, s);
}

void Graph::setEdgeState(const edge_id_t e, const edge_state_t s)
{
	const edge_state_t old = edgeState(e);
	if (s == old)
		return;
	edges_.setCategory(e, s);
	afterEdgeStateChange(e, old, s);
}

node_id_t Graph::addNode()
{
	return addNode(0);
}

node_id_t Graph::addNode(const node_state_t s)
{
	assert(s < nodes_.numberOfCategories());
	// FIXME this is an ugly hack
//	node_id_t id = nodes_.insert(0, s);
//	nodes_[id] = elf_->createNode(id);

	node_id_t id = nodes_.nextInsertId();
	node_id_t nid = nodes_.insert(elf_->createNode(id));
	assert(nid == id);
	afterNodeAdd(id);
	return id;
}

edge_id_t Graph::addEdge(const node_id_t source, const node_id_t target)
{
	//edge_id_t id = edges_.insert(0);
	edge_id_t id = edges_.nextInsertId();
	Edge* e = 0;
	try
	{
		// FIXME this is an ugly hack
		e = elf_->createEdge(id, *node(source), *node(target));
		//edges_[id] = e;
		edges_.insert(e);
		afterEdgeAdd(id);
		return id;
	} catch (SingletonException& ex)
	{
		// edge exists and we do not allow multiple edges
//		edges_.erase(id);
//		delete e;
		if (isDirected())
			return node(source)->edgeTo(node(target))->id();
		else
			return node(source)->edgeToAdjacentNode(node(target))->id();
	}
}

void Graph::removeNode(const node_id_t n)
{
	assert(nodes_.valid(n));
	beforeNodeRemove(n);
	nodes_.erase(n);
}

void Graph::removeEdge(const edge_id_t e)
{
	assert(edges_.valid(e));
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
	assert(nodes_.valid(n));
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterNodeAdd(*this, *node(n));
}

void Graph::afterEdgeAdd(const edge_id_t e)
{
	assert(edges_.valid(e));
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterEdgeAdd(*this, *edge(e));
}

void Graph::beforeNodeRemove(const node_id_t n)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->beforeNodeRemove(*this, *node(n));
}

void Graph::beforeEdgeRemove(const edge_id_t e)
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->beforeEdgeRemove(*this, *edge(e));
}

void Graph::beforeGraphClear()
{
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->beforeGraphClear(*this);
}

void Graph::afterNodeStateChange(const node_id_t n,
		const node_state_t oldState, const node_state_t newState)
{
	assert(nodes_.valid(n));
	assert(oldState < nodes_.numberOfCategories());
	assert(newState < nodes_.numberOfCategories());
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterNodeStateChange(*this, *node(n), oldState, newState);
}

void Graph::afterEdgeStateChange(const edge_id_t e,
		const edge_state_t oldState, const edge_state_t newState)
{
	assert(edges_.valid(e));
	assert(oldState < edges_.numberOfCategories());
	assert(newState < edges_.numberOfCategories());
	for (ListenerContainer::iterator i = listeners_.begin(); i
			!= listeners_.end(); ++i)
		(*i)->afterEdgeStateChange(*this, *edge(e), oldState, newState);
}

}
