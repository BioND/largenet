/**
 * @file MultiNode.cpp
 * @date 01.09.2009
 * @author gerd
 */

#include "MultiNode.h"
#include "Edge.h"

namespace largenet
{

bool MultiNode::hasEdgeTo(const Node* n) const
{
	for (edge_set::iterator it = outEdges_.begin(); it != outEdges_.end(); ++it)
	{
		if ((*it)->target() == n)
			return true;
	}
	return false;
}

bool MultiNode::hasEdgeFrom(const Node* n) const
{
	for (edge_set::iterator it = inEdges_.begin(); it != inEdges_.end(); ++it)
	{
		if ((*it)->source() == n)
			return true;
	}
	return false;
}

void MultiNode::registerEdge(Edge* e)
{
	if (hasEdge(e))
		return;
	if (e->source() == this)
		outEdges_.insert(e);
	else if (e->target() == this)
		inEdges_.insert(e);
}

void MultiNode::unregisterEdge(Edge* e)
{
	if (hasInEdge(e))
		inEdges_.erase(e);
	else if (hasOutEdge(e))
		outEdges_.erase(e);
}

}
