/**
 * @file generators.h
 * @author Gerd Zschaler
 * @date Aug 19, 2008
 * Graph generators.
 * @todo Implement Barabasi-Albert scale-free networks, and some form of Watts-Strogatz
 * 		 small-world networks. Maybe some of those special networks (clustering scale-free etc.)
 * 		 could be useful, too.
 */
#ifndef GENERATORS_H_
#define GENERATORS_H_

#include "base/types.h"
#include "base/traits.h"
#include "../myrng1.2/myrngWELL.h"
#include <cassert>

namespace lnet
{

/**
 * Utility namespace containing various network generators.
 */
namespace generators
{

/**
 * Create a random Erdos-Renyi network from @p net with @p nLinks.
 *
 * This function removes all links in the network and then randomly adds @p nLinks links, thus
 * creating an Erdos-Renyi network.
 *
 * @param[in,out] net Network object (containing @p N nodes) to make random network of.
 * @param[in] nLinks  Number of links to create in @p net.
 * @param[in] selfLoops If true, self-loops are allowed.
 */
template<class _Network>
void erdosRenyiNetwork(_Network& net, const id_size_t nLinks,
		const bool selfLoops = false)
{
	id_size_t n = net.numberOfNodes();
	if (n < 1)
		return;
	assert(nLinks <= n*(n-1)/2);
	net.removeAllLinks();
	while (net.numberOfLinks() < nLinks)
	{
		std::pair<bool, node_id_t> a = net.randomNode(), b = net.randomNode();
		if (!selfLoops && (a.second == b.second))
			continue;
		net.addLink(a.second, b.second);
	}
}

/**
 * Create a random Erdos-Renyi network from @p net with link probability @p p.
 *
 * Links are created with probability @p p in such a way that the expected (average)
 * number of links is @f$ \frac{p}{2} N(N-1) @f$. No self-loops or parallel edges are
 * created.
 * @param[in,out] net Network object (containing @p N nodes) to make random network of.
 * @param[in] p Link creation probability.
 */
template<class _Network>
void erdosRenyiNetwork(_Network& net, const double p)
{
	assert(p >= 0.0);
	assert(p <= 1.0);
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();

	for (typename network_traits<_Network>::NodeIterator it = iters.first; it
			!= iters.second; ++it)
	{
		for (typename network_traits<_Network>::NodeIterator j = iters.first; j
				!= it; ++j)
		{
			if (rng.Chance(p))
				net.addLink(*it, *j);
		}
	}
}

/**
 * Create a star graph of @p nSpikes + 1 nodes, where one central hub is connected
 * to all other @p nSpikes nodes.
 * @param[in,out] net Network object, will be cleared before creating the star graph.
 * @param[in] nSpikes Number of spikes of the star.
 * @return %Node ID of hub node.
 */
// template <class _Network>
//node_id_t starGraph(_Network& net, const id_size_t nSpikes);
/*
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
#endif /*GENERATORS_H_*/
