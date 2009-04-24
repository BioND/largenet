/**
 * @file measures.h
 * @author Gerd Zschaler
 * @date Oct 12, 2008
 * Declaration of the lnet::measures class.
 */
#ifndef MEASURES_H_
#define MEASURES_H_

#include <vector>
#include "../largenet.h"
#include "../motifs/TripleMotif.h"

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
void degreeDistribution(const MultiNetwork& net, degree_dist_t& dist,
		node_state_t s);

/**
 * Calculate degree distribution of nodes.
 * @param[in] net %MultiNetwork to calculate degree distribution for.
 * @param[out] dist	@c std::vector to hold the degree distribution
 */
void degreeDistribution(const MultiNetwork& net, degree_dist_t& dist);

/**
 * Calculate state degree distribution of nodes in state @p source, i.e. the distribution of
 * the numbers of links emanating from nodes in state @p source and leading to nodes in state
 * @p target.
 * @param[in] net %MultiNetwork to calculate degree distribution for.
 * @param[out] dist	@c std::vector to hold the degree distribution.
 * @param[in] source	%Node state to calculate the degree distribution for.
 * @param[in] target	Target node state to calculate the degree distribution for.
 */
void degreeDistribution(const MultiNetwork& net, degree_dist_t& dist,
		node_state_t source, node_state_t target);

/**
 * Calculate distribution of average nearest neighbor degrees
 * @param[in] net %MultiNetwork to calculate distribution for.
 * @param[out] dist	@c std::vector to hold distribution
 */
void averageNearestNeighborDegree(const MultiNetwork& net,
		std::vector<double>& dist);

/**
 * Calculate distribution of average nearest neighbor degrees of nodes in state @p state.
 * @param[in] net %MultiNetwork to calculate distribution for.
 * @param[in] state %Node state to calculate distribution for.
 * @param[out] dist	@c std::vector to hold distribution
 */
void averageNearestNeighborDegree(const MultiNetwork& net,
		std::vector<double>& dist, node_state_t state);

/**
 * Calculate the clustering coefficient of node @p n.
 * @param net %MultiNetwork the node is part of.
 * @param n ID of the node
 * @return Clustering coefficient for the node @p n.
 */
double clusteringCoefficient(const MultiNetwork& net, node_id_t n);

/**
 * Calculate the average clustering coefficient for the network. This is the average
 * over the clustering coefficients of all nodes.
 * @param net
 * @return Clustering coefficient.
 */
double clusteringCoefficient(const MultiNetwork& net);

/**
 * Calculate the transitivity (fraction of transitive triangles) of the network.
 * @param net
 * @return %MultiNetwork transitivity.
 */
double transitivity(const MultiNetwork& net);

id_size_t triples(const MultiNetwork& net);
id_size_t triples_slow(const MultiNetwork& net);

template<class LinkStateCalculator>
id_size_t triples(const MultiNetwork& net, const TripleMotif& t,
		LinkStateCalculator& lc)
{
	id_size_t ret = 0;

	// loop over all a-b links
	MultiNetwork::LinkStateIteratorRange iters = net.links(lc(t.left(), t.center()));
	for (MultiNetwork::LinkStateIterator& it = iters.first; it != iters.second; ++it)
	{
		node_id_t left, right;
		if (net.getNodeState(net.source(*it)) == t.left())
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
		MultiNetwork::NeighborIteratorRange niters = net.neighbors(right);
		for (MultiNetwork::NeighborIterator& nit = niters.first; nit
				!= niters.second; ++nit)
		{
			if (net.getNodeState(*nit) == t.right())
				++ret;
		}

		// exclude left a in an a-b-a triple
		if (t.isSymmetric())
			--ret;

		// in an a-a-c triple, do the same thing for the left a node
		if (t.left() == t.center())
		{
			// count all c-neighbors of the left node
			MultiNetwork::NeighborIteratorRange niters = net.neighbors(left);
			for (MultiNetwork::NeighborIterator& nit = niters.first; nit
					!= niters.second; ++nit)
			{
				if (net.getNodeState(*nit) == t.right())
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
