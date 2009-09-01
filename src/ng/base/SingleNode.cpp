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

bool SingleNode::hasEdgeTo(Node* n) const
{
	for (edge_set::const_iterator it = outEdges_.begin(); it != outEdges_.end(); ++it)
	{
		if ((*it)->target() == n)
			return true;
	}
	return false;
}

bool SingleNode::hasEdgeFrom(Node* n) const
{
	for (edge_set::const_iterator it = inEdges_.begin(); it != inEdges_.end(); ++it)
	{
		if ((*it)->source() == n)
			return true;
	}
	return false;
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
