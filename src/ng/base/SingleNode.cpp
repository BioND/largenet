/**
 * @file SingleNode.cpp
 * @date 01.09.2009
 * @author gerd
 */

#include "SingleNode.h"
#include "Edge.h"
#include "exceptions.h"

namespace largenet
{

bool SingleNode::hasEdgeTo(const Node* n) const
{
	for (edge_set::iterator it = outEdges_.begin(); it != outEdges_.end(); ++it)
	{
		if ((*it)->target() == n)
			return true;
	}
	return false;
}

Edge* SingleNode::edgeTo(const Node* n) const
{
	for (edge_set::iterator it = outEdges_.begin(); it != outEdges_.end(); ++it)
	{
		if ((*it)->target() == n)
			return *it;
	}
	return 0;

}

bool SingleNode::hasEdgeFrom(const Node* n) const
{
	for (edge_set::iterator it = inEdges_.begin(); it != inEdges_.end(); ++it)
	{
		if ((*it)->source() == n)
			return true;
	}
	return false;
}

Edge* SingleNode::edgeFrom(const Node* n) const
{
	for (edge_set::iterator it = inEdges_.begin(); it != inEdges_.end(); ++it)
	{
		if ((*it)->source() == n)
			return *it;
	}
	return 0;
}

void SingleNode::registerEdge(Edge* e)
{
	if (hasEdge(e))
		return;
	if (e->source() == this)
	{
		if (hasEdgeTo(e->target()))
			throw SingletonException("Edge already exists.");
		outEdges_.insert(e);
	}
	else if (e->target() == this)
	{
		if (hasEdgeFrom(e->source()))
			throw SingletonException("Edge already exists.");
		inEdges_.insert(e);
	}
}

void SingleNode::unregisterEdge(Edge* e)
{
	if (hasInEdge(e))
		inEdges_.erase(e);
	else if (hasOutEdge(e))
		outEdges_.erase(e);
}

}
