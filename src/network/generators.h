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

#include "types.h"

namespace lnet
{

class MultiNetwork;
class Network;

/**
 * Utility namespace containing various network generators.
 */
namespace generators {

/**
 * Create a random Erdos-Renyi network from @p net with @p nLinks.
 *
 * This function removes all links in the network and then randomly adds @p nLinks links, thus
 * creating an Erdos-Renyi network.
 *
 * If @p parallelLinks is @c false, parallel links (multiple links connecting the same nodes) are disallowed.
 *
 * @param[in,out] net MultiNetwork object (containing @p N nodes) to make random network of.
 * @param[in] nLinks  Number of links to create in @p net.
 * @param[in] selfLoops If true, self-loops are allowed.
 * @param[in] parallelLinks If true, parallel links are allowed.
 */
void erdosRenyiNetwork(MultiNetwork& net, id_size_t nLinks, bool selfLoops = false,
		bool parallelLinks = false);

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
void erdosRenyiNetwork(Network& net, id_size_t nLinks, bool selfLoops = false);

/**
 * Create a random Erdos-Renyi network from @p net with link probability @p p.
 *
 * Links are created with probability @p p in such a way that the expected (average)
 * number of links is @f$ \frac{p}{2} N(N-1) @f$. No self-loops or parallel edges are
 * created.
 * @param[in,out] net MultiNetwork object (containing @p N nodes) to make random network of.
 * @param[in] p Link creation probability.
 */
void erdosRenyiNetwork(MultiNetwork& net, double p);


/**
 * Create a random Erdos-Renyi network from @p net with link probability @p p.
 *
 * Links are created with probability @p p in such a way that the expected (average)
 * number of links is @f$ \frac{p}{2} N(N-1) @f$. No self-loops or parallel edges are
 * created.
 * @param[in,out] net Network object (containing @p N nodes) to make random network of.
 * @param[in] p Link creation probability.
 */
void erdosRenyiNetwork(Network& net, double p);

/**
 * Create a star graph of @p nSpikes + 1 nodes, where one central hub is connected
 * to all other @p nSpikes nodes.
 * @param[in,out] net Network object, will be cleared before creating the star graph.
 * @param[in] nSpikes Number of spikes of the star.
 * @return %Node ID of hub node.
 */
node_id_t starGraph(MultiNetwork& net, id_size_t nSpikes);

}
}
#endif /*GENERATORS_H_*/
