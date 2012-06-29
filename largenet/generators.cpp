/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
#include "generators.h"
#include <largenet/base/BasicNetwork.h>
#include <utility>
#include <boost/version.hpp>

#if (BOOST_VERSION > 103600)
#include <boost/unordered_set.hpp>
#else
#include <set>
#endif

namespace lnet
{

namespace generators
{

void randomNetworkGnm(BasicNetwork& net, const id_size_t nLinks)
{
	typedef std::pair<node_id_t, node_id_t> edge_t;
#if (BOOST_VERSION > 103600)
	// hash table for edges
	typedef boost::unordered_set<edge_t> edge_set;
#else
	typedef std::set<edge_t> edge_set;
#endif

	id_size_t n = net.numberOfNodes();
	if (n < 1)
		return;
	id_size_t max_edges = n * (n - 1) / 2;
	assert(nLinks <= n * (n - 1) / 2);
	net.removeAllLinks();

	/*
	 * efficient G(n,m) from Phys. Rev. E 71, 036113 (2005)
	 */

	edge_set edges;
	edge_t current_edge;
	for (id_size_t i = 0; i < nLinks; ++i)
	{
		while (true)
		{
			id_size_t from = 0, to = max_edges - 1;
			id_size_t edge_index = rng.IntFromTo(from, to);

			current_edge.first = 1
					+ static_cast<node_id_t>(std::floor(
							std::sqrt(0.25 + 2.0 * edge_index) - 0.5));
			current_edge.second = static_cast<node_id_t>(edge_index
					- current_edge.first * (current_edge.first - 1) / 2);
			if (edges.find(current_edge) == edges.end())
			{
				edges.insert(current_edge);
				net.addLink(current_edge.first, current_edge.second);
				break;
			}
		}
	}
}

}
}
