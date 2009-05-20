#include "generators.h"
#include "../myrng1.2/myrngWELL.h"
#include "MultiNetwork.h"
#include "Network.h"
#include <cassert>

namespace lnet
{

namespace generators
{

void erdosRenyiNetwork(MultiNetwork& net, const id_size_t nLinks,
		const bool selfLoops, const bool parallelLinks)
{
	id_size_t n = net.numberOfNodes();
	if (n < 1)
		return;
	assert(nLinks <= n*(n-1)/2);
	net.removeAllLinks();
	while (net.numberOfLinks() < nLinks)
	{
		std::pair<bool, node_id_t> a = net.randomNode(), b = net.randomNode();
		if (net.isLink(a.second, b.second).first && !parallelLinks)
			continue;
		if ((a.second == b.second) && !selfLoops)
			continue;
		net.addLink(a.second, b.second);
	}
}

void erdosRenyiNetwork(Network& net, const id_size_t nLinks,
		const bool selfLoops)
{
	id_size_t n = net.numberOfNodes();
	if (n < 1)
		return;
	assert(nLinks <= n*(n-1)/2);
	net.removeAllLinks();
	while (net.numberOfLinks() < nLinks)
	{
		std::pair<bool, node_id_t> a = net.randomNode(), b = net.randomNode();
		// we don't need to check if the link exists, since Network will
		// silently ignore double links, and we loop until the network has
		// the desired number of links
		if ((a.second == b.second) && !selfLoops)
			continue;
		net.addLink(a.second, b.second);
	}
}

void erdosRenyiNetwork(MultiNetwork& net, const double p)
{
	assert(p >= 0.0);
	assert(p <= 1.0);
	MultiNetwork::NodeIteratorRange iters = net.nodes();

	for (MultiNetwork::NodeIterator it = iters.first; it != iters.second; ++it)
	{
		for (MultiNetwork::NodeIterator j = iters.first; j != it; ++j)
		{
			if (rng.Chance(p))
				net.addLink(*it, *j);
		}
	}
}

void erdosRenyiNetwork(Network& net, const double p)
{
	assert(p >= 0.0);
	assert(p <= 1.0);
	MultiNetwork::NodeIteratorRange iters = net.nodes();

	for (MultiNetwork::NodeIterator it = iters.first; it != iters.second; ++it)
	{
		for (MultiNetwork::NodeIterator j = iters.first; j != it; ++j)
		{
			if (rng.Chance(p))
				net.addLink(*it, *j);
		}
	}
}

/*node_id_t starGraph(MultiNetwork& net, const id_size_t nSpikes)
{
	node_state_size_t ns = net.numberOfNodeStates();
	link_state_size_t ls = net.numberOfLinkStates();
	net.reset(nSpikes + 1, nSpikes, ns, ls, net.getLinkStateCalculator());
	MultiNetwork::NodeIteratorRange iters = net.nodes();
	assert(iters.first != iters.second);
	node_id_t hub = *iters.first;
	++iters.first;
	for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		net.addLink(hub, *it);
	}
	return hub;
}
*/
}
}
