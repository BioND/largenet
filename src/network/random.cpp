#include "random.h"
#include "MultiNetwork.h"
#include "../myrng1.2/myrngWELL.h"

namespace lnet
{
node_id_t randomNode(const MultiNetwork& net)
{
	const int i = rng.IntFromTo(0, net.numberOfNodes() - 1);
	MultiNetwork::NodeIteratorRange iters = net.nodes();
	for (int j = 0; j < i; ++j)
	{
		++iters.first;
	}
	// this is safe since a valid IndexIterator will always dereference
	// to an existing item
	return *iters.first;
}

node_id_t randomNode(const MultiNetwork& net, const node_state_t s)
{
	const int i = rng.IntFromTo(0, net.numberOfNodes(s) - 1);
	MultiNetwork::NodeStateIteratorRange iters = net.nodes(s);
	for (int j = 0; j < i; ++j)
	{
		++iters.first;
	}
	// this is safe since a valid IndexIterator will always dereference
	// to an existing item
	return *iters.first;
}

link_id_t randomLink(const MultiNetwork& net)
{
	const int i = rng.IntFromTo(0, net.numberOfLinks() - 1);
	MultiNetwork::LinkIteratorRange iters = net.links();
	for (int j = 0; j < i; ++j)
	{
		++iters.first;
	}
	// this is safe since a valid IndexIterator will always dereference
	// to an existing item
	return *iters.first;
}

link_id_t randomLink(const MultiNetwork& net, const link_state_t s)
{
	const int i = rng.IntFromTo(0, net.numberOfLinks(s) - 1);
	MultiNetwork::LinkStateIteratorRange iters = net.links(s);
	for (int j = 0; j < i; ++j)
	{
		++iters.first;
	}
	// this is safe since a valid IndexIterator will always dereference
	// to an existing item
	return *iters.first;
}

void randomizeNodeStates(MultiNetwork& net)
{
	const node_state_size_t m = net.numberOfNodeStates() - 1;
	MultiNetwork::NodeIteratorRange iters = net.nodes();
	for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		net.setNodeState(*it, rng.IntFromTo(0, m));
	}
}

}
