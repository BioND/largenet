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
