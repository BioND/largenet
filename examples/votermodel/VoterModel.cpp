/**
 * @file VoterModel.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "VoterModel.h"
#include <largenet/myrng1.2/myrngWELL.h>
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
	if (net_.numberOfNodes() < 2)
		return 1000;

	const double tau = 1.0 / net_.numberOfLinks();

	pair<bool, node_id_t> n1 = net_.randomNode();
	pair<bool, link_id_t> l2 = net_.randomNeighborLink(n1.second);
	if (!n2.first)
		return tau;

	node_id_t n2 = (net_.source(l2.second) == n1.second) ? net_.target(l2.second) : net_.source(l2.second);

	if (net_.nodeState(n1.second) == net_.nodeState(n2.second))
		return tau;

	if (rng.Chance(p_))
	{
		rewire(l2.second, n1.second);
	}
	else
		net_.setNodeState(n1.second, net_.nodeState(n2.second));
	return tau;
}

void VoterModel::rewire(link_id_t l, node_id_t source)
{
	node_state_t s = net_.nodeState(source);
	if (net_.numberOfNodes(s) == 1)
		return;

	unsigned int tries = 0;
	const unsigned int max_tries = net_.numberOfNodes(s);
	while (tries < max_tries)
	{
		pair<bool, node_id_t> n2 = net_.randomNode(s); // same state
		if (n2.second == source) // disallow self-loops
		{
			++tries;
			continue;
		}
		if (net_.changeLink(l, source, n2.second))
			break;
		else
			++tries;
	}
}

