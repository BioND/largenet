/**
 * @file Edge.cpp
 * @date 01.09.2009
 * @author gerd
 */

#include "Edge.h"
#include "Node.h"

namespace largenet
{

Edge::Edge() :
	id_(0), source_(0), target_(0)
{
}

Edge::Edge(const edge_id_t id, Node* source, Node* target) :
	id_(id), source_(source), target_(target)
{
	connect();
}

Edge::~Edge()
{
	disconnect();
}

Node* Edge::opposite(const Node* from) const
{
	if (from == source_)
		return target_;
	else if (from == target_)
		return source_;
	else
		return 0;
}

void Edge::connect()
{
	if ((source_ == 0) || (target_ == 0))
		return;
	source_->registerEdge(this);
	target_->registerEdge(this);
}

void Edge::disconnect()
{
	if ((source_ == 0) || (target_ == 0))
		return;
	source_->unregisterEdge(this);
	target_->unregisterEdge(this);
}

}
