#include "MultiNetwork.h"
#include "../myrng1.2/myrngWELL.h"

namespace lnet
{

MultiNetwork::MultiNetwork()
{
	nodeStore_ = new NodeRepo(1, 0);
	linkStore_ = new LinkRepo(1, 0);
}

MultiNetwork::MultiNetwork(const id_size_t nNodes, const id_size_t nLinks,
		const id_size_t nNodeStates, const id_size_t nLinkStates)
{
	nodeStore_ = new NodeRepo(nNodeStates, nNodes);
	linkStore_ = new LinkRepo(nLinkStates, nLinks);
	init(nNodes);
}

MultiNetwork::~MultiNetwork()
{
	clear();
	delete nodeStore_;
	delete linkStore_;
}

void MultiNetwork::init(id_size_t nodes)
{
	if (nodeStore_->size() > 0)
		nodeStore_->deleteAll();
	if (linkStore_->size() > 0)
		linkStore_->deleteAll();
	for (id_size_t i = 0; i < nodes; ++i)
	{
		(*nodeStore_) << new Node();
	}
}

void MultiNetwork::reset(const id_size_t nNodes, const id_size_t nLinks,
		const node_state_size_t nNodeStates,
		const link_state_size_t nLinkStates)
{
	clear();
	delete nodeStore_;
	delete linkStore_;
	nodeStore_ = new NodeRepo(nNodeStates, nNodes);
	linkStore_ = new LinkRepo(nLinkStates, nLinks);
	init(nNodes);
}

void MultiNetwork::clear()
{
	nodeStore_->deleteAll();
	linkStore_->deleteAll();
}

link_id_t MultiNetwork::addLink(const node_id_t source, const node_id_t target)
{
	return addLink(source, target, 0);
}

link_id_t MultiNetwork::addLink(const node_id_t source, const node_id_t target,
		const link_state_t s)
{
	link_id_t l;
	l = linkStore_->insert(new Link(source, target), s);
	node(source).addLink(l);
	node(target).addLink(l);
	return l;
}

bool MultiNetwork::changeLink(const link_id_t l, const node_id_t source,
		const node_id_t target, const link_state_t s)
{
	Link& theLink = link(l);
	if ((source != theLink.source()) || (target != theLink.target()))
	{
		node(theLink.source()).removeLink(l);
		node(theLink.target()).removeLink(l);
		theLink.setSource(source);
		node(source).addLink(l);
		theLink.setTarget(target);
		node(target).addLink(l);
	}
	if (getLinkState(l) != s)
		setLinkState(l, s);
	return true;
}

void MultiNetwork::removeLink(const link_id_t l)
{
	Link& theLink = link(l);
	node(theLink.source()).removeLink(l);
	node(theLink.target()).removeLink(l);
	delete linkStore_->remove(l);
}

void MultiNetwork::removeAllLinks()
{
	NodeIteratorRange iters = nodes();
	for (NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		node(*it).clear();
	}
	linkStore_->deleteAll();
}

void MultiNetwork::removeNode(const node_id_t n)
{
	Node::iter_range iters = node(n).links();
	for (Node::LinkIDIterator& li = iters.first; li != iters.second; ++li)
	{
		// remove neighbors' references to connecting links
		Link& l = link(*li);
		node_id_t m = (l.source() == n) ? l.target() : l.source();
		node(m).removeLink(*li);
		// delete connecting links
		delete linkStore_->remove(*li);
	}
	delete nodeStore_->remove(n);
}

node_id_t MultiNetwork::addNode()
{
	return addNode(0);
}

node_id_t MultiNetwork::addNode(const node_state_t s)
{
	//	return nodeStore_.insert(new Node(s), s);
	return nodeStore_->insert(new Node(), s);
}

void MultiNetwork::setNodeState(const node_id_t n, const node_state_t s)
{
	//	node(n).setState(s);
	nodeStore_->setCategory(n, s);
}

void MultiNetwork::setLinkState(const link_id_t l, const link_state_t s)
{
	//	link(l).setState(s);
	linkStore_->setCategory(l, s);
}

std::pair<bool, link_id_t> MultiNetwork::isLink(const node_id_t source,
		const node_id_t target) const
{
	node_id_t a, b;
	if (node(source).degree() < node(target).degree())
	{
		a = source;
		b = target;
	}
	else
	{
		a = target;
		b = source;
	}

	Node::iter_range iters = node(a).links();
	if (a == b)
	{
		for (Node::LinkIDIterator& it = iters.first; it != iters.second; ++it)
		{
			if (link(*it).isLoop())
			{
				return std::make_pair(true, *it);
			}
		}
	}
	else
	{
		for (Node::LinkIDIterator& it = iters.first; it != iters.second; ++it)
		{
			if (link(*it).connectsTo(b))
			{
				return std::make_pair(true, *it);
			}
		}
	}
	return std::make_pair(false, 0);
}

std::pair<bool, node_id_t> MultiNetwork::randomNode() const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (nodeStore_->size() > 0)
	{
		ret.first = true;
		ret.second = nodeStore_->id(
				static_cast<repo::address_t> (rng.IntFromTo(0,
						nodeStore_->size() - 1)));
	}
	return ret;
}

std::pair<bool, node_id_t> MultiNetwork::randomNode(const node_state_t s) const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (nodeStore_->count(s) > 0)
	{
		ret.first = true;
		ret.second = nodeStore_->id(s,
				static_cast<repo::address_t> (rng.IntFromTo(0,
						nodeStore_->count(s) - 1)));
	}
	return ret;
}

std::pair<bool, link_id_t> MultiNetwork::randomLink() const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (linkStore_->size() > 0)
	{
		ret.first = true;
		ret.second = linkStore_->id(
				static_cast<repo::address_t> (rng.IntFromTo(0,
						linkStore_->size() - 1)));
	}
	return ret;
}

std::pair<bool, link_id_t> MultiNetwork::randomLink(const link_state_t s) const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (linkStore_->count(s) > 0)
	{
		ret.first = true;
		ret.second = linkStore_->id(s,
				static_cast<repo::address_t> (rng.IntFromTo(0,
						linkStore_->count(s) - 1)));
	}
	return ret;
}

std::pair<bool, link_id_t> MultiNetwork::randomNeighborLink(const node_id_t n) const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (degree(n) != 0)
	{
		ret.first = true;
		const int num = rng.IntFromTo(0, degree(n));
		int i = 0;
		NeighborLinkIteratorRange iters = neighborLinks(n);
		for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
		{
			if (i == num)
			{
				ret.second = *it;
				break;
			}
			++i;
		}
	}
	return ret;
}

std::pair<bool, node_id_t> MultiNetwork::randomNeighbor(const node_id_t n) const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (degree(n) != 0)
	{
		ret.first = true;
		const int num = rng.IntFromTo(0, degree(n));
		int i = 0;
		NeighborIteratorRange iters = neighbors(n);
		for (NeighborIterator& it = iters.first; it != iters.second; ++it)
		{
			if (i == num)
			{
				ret.second = *it;
				break;
			}
			++i;
		}
	}
	return ret;
}

MultiNetwork::NeighborIteratorRange MultiNetwork::neighbors(const node_id_t n) const
{
	return std::make_pair(NeighborIterator(*this, n, node(n).links().first),
			NeighborIterator(*this, n, node(n).links().second));
}

MultiNetwork::NeighborLinkIteratorRange MultiNetwork::neighborLinks(
		const node_id_t n) const
{
	return node(n).links();
}

//================ NeighborIterator implementation ======================
MultiNetwork::NeighborIterator::NeighborIterator() :
	net_(0), n_(0)
{
}

MultiNetwork::NeighborIterator::NeighborIterator(const MultiNetwork& net,
		const node_id_t n, const Node::LinkSet::iterator val) :
	net_(&net), n_(n), cur_(val)
{
}

MultiNetwork::NeighborIterator::NeighborIterator(const NeighborIterator& it) :
	net_(it.net_), n_(it.n_), cur_(it.cur_)
{
}

MultiNetwork::NeighborIterator::~NeighborIterator()
{
}

MultiNetwork::NeighborIterator& MultiNetwork::NeighborIterator::operator=(
		const MultiNetwork::NeighborIterator& it)
{
	net_ = it.net_;
	n_ = it.n_;
	cur_ = it.cur_;
	return *this;
}

bool MultiNetwork::NeighborIterator::operator==(
		const MultiNetwork::NeighborIterator& it)
{
	return (net_ == it.net_) && (cur_ == it.cur_) && (n_ == it.n_);
}

bool MultiNetwork::NeighborIterator::operator!=(
		const MultiNetwork::NeighborIterator& it)
{
	return !this->operator==(it);
}

MultiNetwork::NeighborIterator& MultiNetwork::NeighborIterator::operator++()
{
	if (net_)
	{
		++cur_;
	}
	return *this;
}

MultiNetwork::NeighborIterator MultiNetwork::NeighborIterator::operator++(int)
{
	NeighborIterator tmp(*this);
	++(*this);
	return (tmp);
}

node_id_t MultiNetwork::NeighborIterator::operator*()
{
	if (net_)
	{
		const Link& theLink = net_->link(*cur_);
		return (theLink.source() == n_) ? theLink.target() : theLink.source();
	}
	else
	{
		return 0; // FIXME is this wise? singular iterator should not be dereferenced anyway.
	}
}

}
