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

Edge* MultiNode::edgeTo(const Node* n) const
{

	for (edge_set::iterator it = outEdges_.begin(); it != outEdges_.end(); ++it)
	{
		if ((*it)->target() == n)
			return *it;
	}
	throw(std::invalid_argument("Node has no edge to given node."));
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

Edge* MultiNode::edgeFrom(const Node* n) const
{
	for (edge_set::iterator it = inEdges_.begin(); it != inEdges_.end(); ++it)
	{
		if ((*it)->source() == n)
			return *it;
	}
	throw(std::invalid_argument("Node has no edge from given node."));
}

void MultiNode::registerEdge(Edge* e)
{
	if (hasEdge(e))
		return;
	if ((e->source() == this) || (!e->isDirected()))
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
