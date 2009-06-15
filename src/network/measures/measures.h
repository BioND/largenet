/**
 * @file measures.h
 * @author Gerd Zschaler
 * @date Oct 12, 2008
 * Declaration of the lnet::measures class.
 */
#ifndef MEASURES_H_
#define MEASURES_H_

#include "../base/types.h"
#include "../base/traits.h"
#include "../motifs/TripleMotif.h"
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
	return static_cast<double> (triangles) * 6 / (deg * (deg - 1));
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
		if (d > 0)
			t += d * (d - 1);
	}
	return t / 2;
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
			net.links(net.getLinkStateCalculator()(t.left(), t.center()));
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

}

}

#endif /* MEASURES_H_ */
