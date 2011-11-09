/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

/**
 * @file MultiNetwork.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::MultiNetwork class.
 */
#ifndef MULTINETWORK_H_
#define MULTINETWORK_H_

#include "base/Node.h"
#include "base/Link.h"
#include "base/TypedNetwork.h"

namespace lnet
{

/**
 * A class representing a network of Nodes connected by Links. Parallel edges (multiple
 * links connecting the same nodes) and self-loops are allowed. %Nodes and links can have
 * different states that are automatically kept consistent using a LinkStateCalculator object.
 *
 * @todo We need some network class hierarchy representing directed/undirected graphs
 * 		 where parallel edges may be (dis)allowed, and self-loops, too. See boost::graph.
 */
typedef TypedNetwork<Node, Link> MultiNetwork;

}

#endif /*MULTINETWORK_H_*/
