/**
 * @file random.h
 * @date Aug 20, 2008
 * @author Gerd Zschaler
 * Functions for randomized access to lnet::MultiNetwork nodes/links.
 * @todo Separating random access functions from lnet::MultiNetwork is probably good style
 * 		 but bad performance, since we have to rely on the iterator interface. It does
 * 		 not look like a good idea to increment an index iterator a thousand times just
 * 	     to reach a random node.
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include "types.h"
#include "MultiNetwork.h"

namespace lnet
{

/**
 * Get random node in MultiNetwork @p net.
 * @param net %MultiNetwork to get random node from.
 * @return Unique ID of random node.
 */
node_id_t randomNode(const MultiNetwork& net);
/**
 * Get random node in state @p s in MultiNetwork @p net.
 * @param net %MultiNetwork to get random node from.
 * @param s %Node state to choose node from.
 * @return Unique ID of random node.
 */
node_id_t randomNode(const MultiNetwork& net, node_state_t s);

/**
 * Get random link in MultiNetwork @p net.
 * @param net %MultiNetwork to get random link from.
 * @return Unique ID of random link.
 */
link_id_t randomLink(const MultiNetwork& net);
/**
 * Get random link in state @p s in MultiNetwork @p net.
 * @param net %MultiNetwork to get random link from.
 * @param s %Link state to choose link from.
 * @return Unique ID of random link.
 */
link_id_t randomLink(const MultiNetwork& net, link_state_t s);

/**
 * Randomly distribute the possible node states in the MultiNetwork @p net.
 * Note that, if Link states depend on node states in the network, they need
 * to be recalculated after this function was called.
 * @param[in,out] net %MultiNetwork to randomize node states in.
 */
void randomizeNodeStates(MultiNetwork& net);

}
#endif /* RANDOM_H_ */
