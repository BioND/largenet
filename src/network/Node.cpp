#include "Node.h"

namespace lnet
{

Node::Node()
{
}

//Node::Node(const node_state_t state) :
//	links_(), state_(state)
//{
//}

Node::~Node()
{
}

void Node::addLink(const link_id_t l)
{
	links_.insert(l);
}

void Node::removeLink(const link_id_t l)
{
	LinkSet::iterator it = links_.find(l);
	if (it != links_.end())
		links_.erase(it);
}

void Node::clear()
{
	links_.clear();
}

bool Node::hasLink(const link_id_t l) const
{
	return links_.find(l) != links_.end();
}

}
