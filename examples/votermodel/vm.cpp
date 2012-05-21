/**
 * @file vm.cpp
 * @author Gerd Zschaler <gzschaler@googlemail.com>
 *
 * Voter model example
 *
 * This is a simulation of the adaptive voter model as in Vazquez et al. (2008), Phys. Rev. Lett. 100, 108702.
 * A network of @p N nodes, representing agents, and @p L links, representing social connections, is
 * created. Each agent holds one of two possible opinions, @p UP or @p DOWN. The system evolves as
 * follows: In each update step a random node and a random one of its neighbors are selected. If
 * they share the same opinion, nothing happens. If they have different opinions, the first node
 * adopts the opinion of its neighbor with probability <tt>(1-p)</tt>. Otherwise, with probability
 * @p p, it cuts its link to the neighbor and rewires it to a random node holding the same opinion.
 *
 * Depending on the parameter @p p either global consensus is reached, in which every node holds the
 * same opinion, or the network splits into two disconnected clusters of nodes holding opposing opinions.
 * For low @p p consensus is always achieved, while for large @p p fragmentation is observed.
 * Fragmentation sets in at a critical value of @p p, which only depends on the average degree of
 * the network, i.e., the ratio of @p L and @p N.
 */

#include "VoterModel.h"

#include <largenet.h>
#include <largenet/generators.h>
#include <iostream>

using namespace std;
using namespace lnet;

int main(int argc, char **argv)
{
	unsigned int N = 1000; // number of nodes
	unsigned int L = 2000; // number of links

	double initial_up = 0.5; // initial fraction of UP-nodes

	double p = 0.3; // rewiring rate

	double tmax = 1000; // maximum simulation time

	// Create a Network object with N nodes, reserved memory for L links (soft limit),
	// and VoterModel::node_states possible node states. The network does not contain
	// any links so far, only disconnected nodes.
	Network net(N, L, VoterModel::node_states);

	// Connect the nodes randomly to form a random, Erdős-Rényi type network.
	generators::randomNetworkGnm(net, L);

	// Iterate over all nodes in the network
	Network::NodeIteratorRange nodes = net.nodes();
	for (Network::NodeIterator n = nodes.first; n != nodes.second; ++n)
		// Set node state to VoterModel::UP for each node
		net.setNodeState(*n, VoterModel::UP);

	const id_size_t initialDownNodes = static_cast<id_size_t>(round(
			net.numberOfNodes() * (1.0 - initial_up)));

	// Set the first initialDownNodes' node states to VoterModel::DOWN.
	// Note that this is fine, since the links among the nodes have been placed
	// randomly, i.e., disregarding the node order.
	Network::NodeIterator n = nodes.first;
	while (net.numberOfNodes(VoterModel::DOWN) < initialDownNodes)
	{
		net.setNodeState(*n, VoterModel::DOWN);
		++n;
	}

	// Create a VoterModel object that uses the Network object and works with
	// the rewiring rate p
	VoterModel model(net, p);

	// Write output header line
	// (time - up nodes - down nodes - up-up links - up-down links - down-down links)
	std::cout << "# t\tup\tdown\tuu\tud\tdd\n";

	// Simulation loop
	// Perform model steps until simulation time exceeds maximum or the model
	// has stopped (no further changes possible)
	double t = 0, next = 0;
	double interval = 1; // output interval
	while ((t <= tmax) && (!model.stopped()))
	{
		// only write output at specified intervals
		if (t >= next)
		{
			std::cout << t << "\t"
					<< net.numberOfNodes(VoterModel::UP) << "\t"
					<< net.numberOfNodes(VoterModel::DOWN) << "\t"
					<< net.numberOfLinks(VoterModel::UU) << "\t"
					<< net.numberOfLinks(VoterModel::UD) << "\t"
					<< net.numberOfLinks(VoterModel::DD) << "\n";
			next += interval;
		}
		// One simulation step
		t += model.step();
	}

	return 0;
}
