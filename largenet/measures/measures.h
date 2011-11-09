/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file measures.h
 * @author Gerd Zschaler
 * @date Oct 12, 2008
 * Declaration of the lnet::measures class.
 */
#ifndef MEASURES_H_
#define MEASURES_H_

#include <largenet/base/types.h>
#include <largenet/base/traits.h>
#include <largenet/motifs/TripleMotif.h>
#include <largenet/motifs/QuadLineMotif.h>
#include <largenet/motifs/QuadStarMotif.h>
#include <largenet/TripleNetwork.h>
#include <largenet/TripleMultiNetwork.h>
#include <vector>
#include <cassert>

namespace lnet
{

/**
 * Utility namespace providing several common graph measures, e.g. degree distribution etc.
 */
namespace measures
{

typedef std::vector<id_size_t> degree_dist_t; ///< Type of degree distribution histograms (arrays).

/**
 * Calculate degree distribution of nodes in state @p s.
 * @param[in] net %MultiNetwork to calculate degree distribution for.
 * @param[out] dist	@c std::vector to hold the degree distribution.
 * @param[in] s	Node state to calculate the degree distribution for.
 */
template<class _Network>
void degreeDistribution(const _Network& net, degree_dist_t& dist,
		node_state_t s)
{
	dist.clear();
	typename network_traits<_Network>::NodeStateIteratorRange iters =
			net.nodes(s);
	for (typename network_traits<_Network>::NodeStateIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		id_size_t deg = net.degree(*it);
		while (deg >= dist.size())
		{
			dist.push_back(0);
		}
		++dist[deg];
	}
}

/**
 * Calculate degree distribution of nodes.
 * @param[in] net %MultiNetwork to calculate degree distribution for.
 * @param[out] dist	@c std::vector to hold the degree distribution
 */
template<class _Network>
void degreeDistribution(const _Network& net, degree_dist_t& dist)
{
	dist.clear();
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	for (typename network_traits<_Network>::NodeIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		id_size_t deg = net.degree(*it);
		while (deg >= dist.size())
		{
			dist.push_back(0);
		}
		++dist[deg];
	}
}

/**
 * Calculate state degree distribution of nodes in state @p source, i.e. the distribution of
 * the numbers of links emanating from nodes in state @p source and leading to nodes in state
 * @p target.
 * @param[in] net %MultiNetwork to calculate degree distribution for.
 * @param[out] dist	@c std::vector to hold the degree distribution.
 * @param[in] source	%Node state to calculate the degree distribution for.
 * @param[in] target	Target node state to calculate the degree distribution for.
 */
template<class _Network>
void degreeDistribution(const _Network& net, degree_dist_t& dist,
		node_state_t source, node_state_t target)
{
	dist.clear();
	typename network_traits<_Network>::NodeStateIteratorRange iters =
			net.nodes(source);
	for (typename network_traits<_Network>::NodeStateIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		typename network_traits<_Network>::NeighborIteratorRange neighbors =
				net.neighbors(*it);
		id_size_t deg = 0;
		for (typename network_traits<_Network>::NeighborIterator& nit =
				neighbors.first; nit != neighbors.second; ++nit)
		{
			if (net.nodeState(*nit) == target)
				++deg;
		}

		while (deg >= dist.size())
		{
			dist.push_back(0);
		}
		++dist[deg];
	}

}

/**
 * Calculate distribution of average nearest neighbor degrees
 * @param[in] net %MultiNetwork to calculate distribution for.
 * @param[out] dist	@c std::vector to hold distribution
 */
template<class _Network>
void averageNearestNeighborDegree(const _Network& net,
		std::vector<double>& dist)
{
	degree_dist_t degdist;

	// FIXME For better performance, it might be a good idea to calculate this on the fly.
	degreeDistribution(net, degdist);
	std::vector<double> avgNeighborDegrees(degdist.size());

	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	for (typename network_traits<_Network>::NodeIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		id_size_t sum = 0, deg = net.degree(*it);
		typename network_traits<_Network>::NeighborIteratorRange neighbors =
				net.neighbors(*it);
		for (typename network_traits<_Network>::NeighborIterator& nit =
				neighbors.first; nit != neighbors.second; ++nit)
		{
			sum += net.degree(*nit);
		}
		while (deg >= avgNeighborDegrees.size())
		{
			avgNeighborDegrees.push_back(0);
		}
		if (deg > 0)
			avgNeighborDegrees[deg] += static_cast<double> (sum) / deg;
	}

	assert(degdist.size() == avgNeighborDegrees.size());
	dist.clear();
	dist.assign(degdist.size(), 0);
	for (unsigned int i = 0; i < degdist.size(); ++i)
	{
		if (degdist[i] != 0)
			dist[i] = avgNeighborDegrees[i] / degdist[i];
	}
}

/**
 * Calculate distribution of average nearest neighbor degrees of nodes in state @p state.
 * @param[in] net %MultiNetwork to calculate distribution for.
 * @param[in] state %Node state to calculate distribution for.
 * @param[out] dist	@c std::vector to hold distribution
 */
template<class _Network>
void averageNearestNeighborDegree(const _Network& net,
		std::vector<double>& dist, node_state_t state)
{
	degree_dist_t degdist;

	// FIXME For better performance, it might be a good idea to calculate this on the fly.
	degreeDistribution(net, degdist, state);

	std::vector<double> avgNeighborDegrees(degdist.size());

	typename network_traits<_Network>::NodeStateIteratorRange iters =
			net.nodes(state);
	for (typename network_traits<_Network>::NodeStateIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		id_size_t sum = 0, deg = net.degree(*it);
		typename network_traits<_Network>::NeighborIteratorRange neighbors =
				net.neighbors(*it);
		for (typename network_traits<_Network>::NeighborIterator& nit =
				neighbors.first; nit != neighbors.second; ++nit)
		{
			sum += net.degree(*nit);
		}
		while (deg >= avgNeighborDegrees.size())
		{
			avgNeighborDegrees.push_back(0);
		}
		if (deg > 0)
			avgNeighborDegrees[deg] += static_cast<double> (sum) / deg;
	}

	assert(degdist.size() == avgNeighborDegrees.size());
	dist.clear();
	dist.assign(degdist.size(), 0);
	for (unsigned int i = 0; i < degdist.size(); ++i)
	{
		if (degdist[i] != 0)
			dist[i] = avgNeighborDegrees[i] / degdist[i];
	}
}

/**
 * Calculate the clustering coefficient of node @p n.
 * @param net %MultiNetwork the node is part of.
 * @param n ID of the node
 * @return Clustering coefficient for the node @p n.
 */
template<class _Network>
double clusteringCoefficient(const _Network& net, node_id_t n)
{
	const id_size_t deg = net.degree(n);
	if (deg <= 1)
		return 0;

	id_size_t triangles = 0;

	typename network_traits<_Network>::NeighborIteratorRange neighbors =
			net.neighbors(n);
	for (typename network_traits<_Network>::NeighborIterator it1 =
			neighbors.first; it1 != neighbors.second; ++it1)
	{
		for (typename network_traits<_Network>::NeighborIterator it2 =
				neighbors.first; it2 != it1; ++it2)
		// this should be different for directed networks
		{
			if (net.isLink(*it1, *it2).first)
				++triangles;
		}
	}
	return static_cast<double> (triangles) * 2 / (deg * (deg - 1));
}

/**
 * Calculate the average clustering coefficient for the network. This is the average
 * over the clustering coefficients of all nodes.
 * @param net
 * @return Clustering coefficient.
 */
template<class _Network>
double clusteringCoefficient(const _Network& net)
{
	if (net.numberOfNodes() == 0)
		return 0;
	double sum = 0;
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	for (typename network_traits<_Network>::NodeIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		sum += clusteringCoefficient(net, *it);
	}
	return sum / net.numberOfNodes();
}

/**
 * Calculate the transitivity (fraction of transitive triangles) of the network.
 * @param net
 * @return %MultiNetwork transitivity.
 */
template<class _Network>
double transitivity(const _Network& net)
{
	unsigned int triangles = 0, contri = 0;
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	for (typename network_traits<_Network>::NodeIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		id_size_t deg = net.degree(*it);
		contri += deg * (deg - 1); // twice the number of possible triangles
		typename network_traits<_Network>::NeighborIteratorRange niters =
				net.neighbors(*it);
		for (typename network_traits<_Network>::NeighborIterator nit1 =
				niters.first; nit1 != niters.second; ++nit1)
		{
			for (typename network_traits<_Network>::NeighborIterator nit2 =
					niters.first; nit2 != nit1; ++nit2)
			{
				if (net.isLink(*nit1, *nit2).first)
					++triangles;
			}
		}
	}
	if (contri == 0)
		return 0;
	else
		return 2.0 * triangles / static_cast<double> (contri);
}

template<class _Network>
id_size_t triples(const _Network& net)
{
	id_size_t t = 0;
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	for (typename network_traits<_Network>::NodeIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		const id_size_t d = net.degree(*it);
		if (d > 1)
			t += d * (d - 1);
	}
	return t / 2;
}

template<>
inline id_size_t triples<TripleNetwork>(const TripleNetwork& net)
{
	return net.numberOfTriples();
}

template<>
inline id_size_t triples(const TripleMultiNetwork& net)
{
	return net.numberOfTriples();
}

template<class _Network>
id_size_t triples_slow(const _Network& net)
{
	id_size_t t = 0;
	typename network_traits<_Network>::LinkIteratorRange iters = net.links();
	for (typename network_traits<_Network>::LinkIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		t += net.degree(net.target(*it)) + net.degree(net.source(*it)) - 2;
	}
	return t / 2;
}

template<class _Network>
id_size_t triples(const _Network& net, const motifs::TripleMotif& t)
{
	id_size_t ret = 0;

	// loop over all a-b links
	typename network_traits<_Network>::LinkStateIteratorRange iters =
			net.links(net.linkStateCalculator()(t.left(), t.center()));
	for (typename network_traits<_Network>::LinkStateIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		node_id_t left, right;
		if (net.nodeState(net.source(*it)) == t.left())
		{
			left = net.source(*it);
			right = net.target(*it);
		}
		else
		{
			left = net.target(*it);
			right = net.source(*it);
		}

		// count all c-neighbors of the b node
		typename network_traits<_Network>::NeighborIteratorRange niters =
				net.neighbors(right);
		for (typename network_traits<_Network>::NeighborIterator& nit =
				niters.first; nit != niters.second; ++nit)
		{
			if (net.nodeState(*nit) == t.right())
				++ret;
		}

		// exclude left a in an a-b-a triple
		if (t.isSymmetric())
			--ret;

		// in an a-a-c triple, do the same thing for the left a node
		if (t.left() == t.center())
		{
			// count all c-neighbors of the left node
			typename network_traits<_Network>::NeighborIteratorRange niters =
					net.neighbors(left);
			for (typename network_traits<_Network>::NeighborIterator & nit =
					niters.first; nit != niters.second; ++nit)
			{
				if (net.nodeState(*nit) == t.right())
					++ret;
			}

			// exclude left a in an a-b-a triple
			if (t.isSymmetric())
				--ret;
		}
	}

	if (t.isSymmetric())
		ret /= 2;
	return ret;
}

template<>
inline id_size_t triples(const TripleMultiNetwork& net, const motifs::TripleMotif& t)
{
	return net.numberOfTriples(net.getTripleStateCalculator()(t.left(),
			t.center(), t.right()));
}

template<>
inline id_size_t triples(const TripleNetwork& net, const motifs::TripleMotif& t)
{
	return net.numberOfTriples(net.getTripleStateCalculator()(t.left(),
			t.center(), t.right()));
}

template<class _Network>
id_size_t quadLines(const _Network& net)
{
	typedef typename network_traits<_Network>::NeighborIteratorRange NIRange;
	typedef typename network_traits<_Network>::NeighborIterator NI;
	typedef typename network_traits<_Network>::LinkIteratorRange LIRange;
	typedef typename network_traits<_Network>::LinkIterator LI;

	// FIXME this is slooow.
	id_size_t ret = 0;
	LIRange iters = net.links();
	for (LI i = iters.first; i != iters.second; ++i)
	{
		NIRange sneighbors = net.neighbors(net.source(*i));
		for (NI sn = sneighbors.first; sn != sneighbors.second; ++sn)
		{
			if (*sn == net.target(*i))
				continue;
			NIRange tneighbors = net.neighbors(net.target(*i));
			for (NI tn = tneighbors.first; tn != tneighbors.second; ++tn)
			{
				if (*tn == net.source(*i))
					continue;
				if (*sn == *tn)
					continue;
				++ret;
			}
		}
	}

	// each i-j link is part of q_i*q_j quads
	//		ret += (net.degree(net.source(*i)) - 1) * (net.degree(net.target(*i))
	//				- 1); // FIXME this counts closed triangles as 3 quad lines!

	return ret;
}

template<class _Network>
id_size_t quadLines(const _Network& net, node_state_t a, node_state_t b,
		node_state_t c, node_state_t d)
{
	typedef typename network_traits<_Network>::LinkStateIteratorRange LSIRange;
	typedef typename network_traits<_Network>::LinkStateIterator LSI;
	typedef typename network_traits<_Network>::NeighborIteratorRange NIRange;
	typedef typename network_traits<_Network>::NeighborIterator NI;

	id_size_t ret = 0;
	LSIRange iters = net.links(net.linkStateCalculator()(b, c));
	for (LSI i = iters.first; i != iters.second; ++i)
	{
		node_id_t b_node = net.nodeState(net.source(*i)) == b ? net.source(*i)
				: net.target(*i);
		node_id_t c_node = b_node == net.source(*i) ? net.target(*i)
				: net.source(*i);
		assert(b_node != c_node);
		NIRange bneighbors = net.neighbors(b_node);
		for (NI n = bneighbors.first; n != bneighbors.second; ++n)
		{
			if (*n == c_node)
				continue;
			if (net.nodeState(*n) == a)
			{
				NIRange cneighbors = net.neighbors(c_node);
				for (NI m = cneighbors.first; m != cneighbors.second; ++m)
				{
					if (*m == b_node)
						continue;
					if (*m == *n) // do not count closed triangles as quad lines
						continue;
					if (net.nodeState(*m) == d)
						++ret;
				}
			}

			// symmetric center, check swapped ends too
			if ((b == c) && (a != d))
			{
				if (net.nodeState(*n) == d)
				{
					NIRange cneighbors = net.neighbors(c_node);
					for (NI m = cneighbors.first; m != cneighbors.second; ++m)
					{
						if (*m == b_node)
							continue;
						if (*m == *n) // do not count closed triangles as quad lines
							continue;
						if (net.nodeState(*m) == a)
							++ret;
					}
				}
			}
		}
	}
	return ret;
}

/*
 * FIXME this is not trivial
template<>
id_size_t quadLines(const TripleNetwork& net, node_state_t a, node_state_t b,
		node_state_t c, node_state_t d)
{
	id_size_t ret = 0;
	TripleNetwork::TripleStateIteratorRange iters = net.triples(
			net.getTripleStateCalculator()(a, b, c));
	for (TripleNetwork::TripleStateIterator i = iters.first; i != iters.second; ++i)
	{
		node_id_t a_node = net.nodeState(net.leftNode(*i)) == a ? net.leftNode(
				*i) : net.rightNode(*i);
		node_id_t c_node = a_node == net.leftNode(*i) ? net.rightNode(*i)
				: net.leftNode(*i);
		assert(a_node != c_node);
		TripleNetwork::NeighborIteratorRange c_neighbors =
				net.neighbors(c_node);
		for (TripleNetwork::NeighborIterator n = c_neighbors.first; n
				!= c_neighbors.second; ++n)
		{
			if (*n == net.centerNode(*i))
				continue;
			if (*n == a_node) // do not count closed triangles as quad lines
				continue;
			if (net.nodeState(*n) == d)
				++ret;
		}

		if (a == c)
		{
			TripleNetwork::NeighborIteratorRange a_neighbors = net.neighbors(
					a_node);
			for (TripleNetwork::NeighborIterator n = a_neighbors.first; n
					!= a_neighbors.second; ++n)
			{
				if (*n == net.centerNode(*i))
					continue;
				if (*n == c_node) // do not count closed triangles as quad lines
					continue;
				if (net.nodeState(*n) == d)
					++ret;
			}
		}
	}
	if (a == c) ret /= 2;
	return ret;
}
*/

template<class _Network>
id_size_t quadLines(const _Network& net, const motifs::QuadLineMotif& q)
{
	return quadLines(net, q.a(), q.b(), q.c(), q.d());
}

template<class _Network>
id_size_t quadStars(const _Network& net)
{
	id_size_t t = 0;
	typename network_traits<_Network>::NodeIteratorRange iters = net.nodes();
	for (typename network_traits<_Network>::NodeIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		const id_size_t d = net.degree(*it);
		if (d > 2)
			t += d * (d - 1) * (d - 2);
	}
	return t / 6;
}

template<class _Network>
id_size_t quadStars(const _Network& net, node_state_t center, node_state_t a,
		node_state_t b, node_state_t c)
{
	typedef typename network_traits<_Network>::NodeStateIteratorRange NSIRange;
	typedef typename network_traits<_Network>::NodeStateIterator NSI;
	typedef typename network_traits<_Network>::LinkStateIteratorRange LSIRange;
	typedef typename network_traits<_Network>::LinkStateIterator LSI;
	typedef typename network_traits<_Network>::NeighborIteratorRange NIRange;
	typedef typename network_traits<_Network>::NeighborIterator NI;

	id_size_t ret = 0;
	NSIRange nodes = net.nodes(center);
	for (NSI i = nodes.first; i != nodes.second; ++i)
	{
		id_size_t an = 0, bn = 0, cn = 0;
		NIRange neighbors = net.neighbors(*i);
		for (NI n = neighbors.first; n != neighbors.second; ++n)
		{
			if (net.nodeState(*n) == a)
				++an;
			if (net.nodeState(*n) == b)
				++bn;
			if (net.nodeState(*n) == c)
				++cn;
		}
		long int q = an;
		if (a == b)
		{
			if (b == c) // fully symmetric
			{
				q *= (an - 1) * (an - 2);
				q /= 6;
			}
			else // only a == b
			{
				q *= (an - 1) * cn;
				q /= 2;
			}
		}
		else if (a == c) // symmetric, but a != b
		{
			q *= (an - 1) * bn;
			q /= 2;
		}
		else if (b == c) // a != b && a != c
		{
			q *= (bn - 1) * bn;
			q /= 2;
		}
		else
			// a != b != c && a != c
			q *= bn * cn;
		if (q > 0)
			ret += q;
	}

	//	// FIXME what if center node has same state as a, b, or c?
	//	id_size_t ret = 0;
	//	LSIRange iters = net.links(net.linkStateCalculator()(a, center));
	//	for (LSI i = iters.first; i != iters.second; ++i)
	//	{
	//		node_id_t a_node =
	//				(net.nodeState(net.source(*i)) == a) ? net.source(*i)
	//						: net.target(*i);
	//		node_id_t center_node = (a_node == net.source(*i)) ? net.target(*i)
	//				: net.source(*i);
	//		NIRange cneighbors = net.neighbors(center_node);
	//		for (NI n1 = cneighbors.first; n1 != cneighbors.second; ++n1)
	//		{
	//			if (*n1 == a_node)
	//				continue;
	//			NI n2 = n1;
	//			++n2;
	//			for (; n2 != cneighbors.second; ++n2)
	//			{
	//				// FIXME what if symmetric?
	//				if (((net.nodeState(*n1) == b) && (net.nodeState(*n2) == c))
	//						|| ((net.nodeState(*n1) == c) && (net.nodeState(*n2)
	//								== b)))
	//					++ret;
	//			}
	//		}
	//	}
	//
	//	if (b == c) ret /= 2;

	return ret;
}

template<class _Network>
id_size_t quadStars(const _Network& net, const motifs::QuadStarMotif& q)
{
	return quadStars(net, q.center(), q.a(), q.b(), q.c());
}

/*
 * FIXME this is not trivial
 *
template<>
id_size_t quadStars<TripleNetwork>(const TripleNetwork& net, node_state_t center, node_state_t a,
		node_state_t b, node_state_t c)
{
	id_size_t ret = 0;
	TripleNetwork::TripleStateIteratorRange iters = net.triples(net.getTripleStateCalculator()(a, center, b));
	for (TripleNetwork::TripleStateIterator i = iters.first; i != iters.second; ++i)
	{
		TripleNetwork::NeighborIteratorRange neighbors = net.neighbors(net.centerNode(*i));
		for (TripleNetwork::NeighborIterator n = neighbors.first; n != neighbors.second; ++n)
		{
			if (*n == net.leftNode(*i))
				continue;
			if (*n == net.rightNode(*i))
				continue;
			if (net.nodeState(*n) == c)
				++ret;
		}
	}
	if (b == c) ret /= 2;
	return ret;
}
*/

}

}

#endif /* MEASURES_H_ */
