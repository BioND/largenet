/**
 * @file path.h
 * @date 14.10.2009
 * @author gerd
 */

#ifndef PATH_H_
#define PATH_H_

#include "../base/types.h"
#include <map>
#include <vector>

namespace lnet
{

namespace measures {

typedef std::map<node_id_t, unsigned int> path_length_map;

/**
 * Calculate shortest path length from @p source to all reachable nodes.
 * @param g %Network to calculate path lengths in. Must provide NeighborIterator, NeighborIteratorRange, and neighbors().
 * @param source Source node ID.
 * @param[out] paths map containing pairs of node IDs and corresponding distances from @p source.
 * @param cutoff Maximum path length to calculate. All paths will be calculated if zero.
 */
template<class _Network>
void singleSourceShortestPathLengths(const _Network& g, const node_id_t source,
		path_length_map& paths, const unsigned int cutoff = 0)
{
	/**
	 * Taken from NetworkX python package.
	 * @see http://networkx.lanl.gov
	 */
	unsigned int level = 0;
	paths.clear();

	typedef std::vector<node_id_t> node_id_vector;
	typedef typename _Network::NeighborIteratorRange NeighborIteratorRange;
	typedef typename _Network::NeighborIterator NeighborIterator;

	node_id_vector nextlevel, thislevel;
	nextlevel.push_back(source);

	while (!nextlevel.empty())
	{
		thislevel = nextlevel;
		nextlevel.clear();
		for (node_id_vector::iterator i = thislevel.begin(); i
				!= thislevel.end(); ++i)
		{
			if (paths.find(*i) == paths.end())
			{
				paths[*i] = level;
				NeighborIteratorRange niters = g.neighbors(*i);
				for (NeighborIterator it = niters.first; it != niters.second; ++it)
					nextlevel.push_back(*it);
			}
		}
		if (cutoff > 0 && cutoff <= level)
			break;
		++level;
	}
}

template<class _Network>
double averageShortestPathLength(const _Network& g)
{
	/**
	 * Taken from NetworkX python package.
	 * @see http://networkx.lanl.gov
	 */
	double avg = 0;
	typename _Network::NodeIteratorRange niters = g.nodes();
	for (typename _Network::NodeIterator it = niters.first; it != niters.second; ++it)
	{
		path_length_map paths;
		unsigned int sum = 0;
		singleSourceShortestPathLengths(g, *it, paths);
		for (path_length_map::iterator i = paths.begin(); i != paths.end(); ++i)
			sum += i->second;
		avg += static_cast<double> (sum) / paths.size();
	}
	return avg / g.numberOfNodes();
}

}

}

#endif /* PATH_H_ */
