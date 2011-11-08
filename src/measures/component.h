/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file component.h
 * @date 14.10.2009
 * @author gerd
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "path.h"
#include "../network/types.h"
#include <vector>
#include <set>
#include <algorithm>

namespace lnet
{
namespace measures
{

typedef std::vector<node_id_vector> component_vector;

namespace detail
{

template<class V>
bool size_compare(const V& a, const V& b)
{
	return a.size() < b.size();
}

template<class V>
bool reverse_size_compare(const V& a, const V& b)
{
	return a.size() > b.size();
}

}

template<class _Network>
void connectedComponents(const _Network& g, component_vector& components)
{
	components.clear();
	std::set<node_id_t> seen;
	typedef typename _Network::NodeIteratorRange NodeIteratorRange;
	typedef typename _Network::NodeIterator NodeIterator;
	path_length_map c;
	node_id_vector keys;
	NodeIteratorRange iters = g.nodes();
	for (NodeIterator i = iters.first; i != iters.second; ++i)
	{
		if (seen.find(*i) == seen.end())
		{
			c.clear();
			keys.clear();
			singleSourceShortestPathLengths(g, *i, c);
			keys.reserve(c.size());
			for (path_length_map::iterator j = c.begin(); j != c.end(); ++j)
			{
				keys.push_back(j->first);
				seen.insert(j->first);
			}
			components.push_back(keys);
		}
	}
	std::sort(components.begin(), components.end(), detail::reverse_size_compare<node_id_vector>);
}

template<class _Network>
size_t numConnectedComponents(const _Network& g)
{
	component_vector c;
	connectedComponents(g, c);
	return c.size();
}

}
}

#endif /* COMPONENT_H_ */
