/**
 * @file VoterModel.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "VoterModel.h"
#include <largenet/myrng/myrngWELL.h> // Makes random number generator instance rng available
#include <utility>

using namespace lnet;
using std::pair;
using std::make_pair;

VoterModel::VoterModel(Network& net, double p) :
		net_(net), p_(p)
{
}

double VoterModel::step()
{
	// do nothing if there is only one node
	if (net_.numberOfNodes() < 2)
		return 1000;

	// here, we measure time in units of 1/L
	const double tau = 1.0 / net_.numberOfLinks();

	// Get a random node from the network. The first member of the returned pair
	// indicates whether this was successful. Only use the second member (the actual
	// node ID) if the first member is true.
	pair<bool, node_id_t> n1 = net_.randomNode();

	// Get a random link to a neighbor of the node. Again, only use the second member if the
	// first member is true.
	pair<bool, link_id_t> l2 = net_.randomNeighborLink(n1.second);
	if (!l2.first)
		return tau;	// do nothing if the node has no neighbors

	// Get the node ID of the neighbor node (we only have the connecting link's ID so far)
	node_id_t n2 = (net_.source(l2.second) == n1.second) ? net_.target(l2.second) : net_.source(l2.second);

	// Do nothing if the nodes share the same state
	if (net_.nodeState(n1.second) == net_.nodeState(n2))
		return tau;

	// Rewire with probability p (we use the random number generator rng)
	if (rng.Chance(p_))
		rewire(l2.second, n1.second);
	// or copy the neighbor's state with probability (1-p)
	else
		net_.setNodeState(n1.second, net_.nodeState(n2));

	// return time increment
	return tau;
}

void VoterModel::rewire(link_id_t l, node_id_t source)
{
	// get source node state
	node_state_t s = net_.nodeState(source);

	// do nothing if there are no nodes (in the same state) we can rewire to
	if (net_.numberOfNodes(s) == 1)
		return;

	// try only so often to rewire a link
	unsigned int tries = 0;
	const unsigned int max_tries = net_.numberOfNodes(s);
	while (tries < max_tries)
	{
		// get a random node in the same state as the source node
		pair<bool, node_id_t> n2 = net_.randomNode(s);

		// disallow the creation of self-loops
		if (n2.second == source)
		{
			++tries;
			continue;
		}

		// Network::changeLink(l, a, b) tries to rewire a link l to connect nodes
		// a and b, it returns true on success, false otherwise (e.g., when nodes
		// a and b are already connected).
		if (net_.changeLink(l, source, n2.second))
			break;
		else
			++tries;
	}
}

