/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
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

#include "network/types.h"
#include "network/traits.h"
#include "myrng1.2/myrngWELL.h"
#include <cassert>
#include <cmath>

namespace lnet
{

class BasicNetwork;

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
 */
void randomNetworkGnm(BasicNetwork& net, const id_size_t nLinks);

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
void randomNetworkGnp(_Network& net, const double p)
{
	/*
	 * efficient G(n,p) from Phys. Rev. E 71, 036113 (2005)
	 */
	assert(p > 0.0);
	assert(p < 1.0);
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	long int w = -1;
	for (typename network_traits<_Network>::NodeIterator v = iters.first; v != iters.second; ++v)
	{
		double r = rng.Uniform01();
		w = 1 + w + static_cast<long int> (std::floor(std::log(1.0 - r)/std::log(1.0 - p)));
		while ((w >= *v) && (v != iters.second))
		{
			w -= *v;
			++v;
		}
		if (v != iters.second)
			net.addLink(*v, w);
	}
//
//
//	for (typename network_traits<_Network>::NodeIterator it = iters.first; it
//			!= iters.second; ++it)
//	{
//		for (typename network_traits<_Network>::NodeIterator j = iters.first; j
//				!= it; ++j)
//		{
//			if (rng.Chance(p))
//				net.addLink(*it, *j);
//		}
//	}
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
