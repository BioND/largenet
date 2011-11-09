
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

	double tmax = 1000;

	Network net(N, L, VoterModel::node_states);
	generators::randomNetworkGnm(net, L);

	Network::NodeIteratorRange nodes = net.nodes();
	for (Network::NodeIterator n = nodes.first; n != nodes.second; ++n)
		net.setNodeState(*n, VoterModel::UP);

	const id_size_t initialDownNodes = static_cast<id_size_t>(round(
			net.numberOfNodes() * (1.0 - initial_up)));

	Network::NodeIterator n = nodes.first;
	while (net.numberOfNodes(VoterModel::DOWN) < initialDownNodes)
	{
		net_->setNodeState(*n, VoterModel::DOWN);
		++n;
	}

	VoterModel model(net, p);

	std::cout << "# t\tup\tdown\tuu\tud\tdd\n";

	double t = 0, next = 0;
	double interval = 1; // output interval
	while ((t <= tmax) && (!model_->stopped()))
	{
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
		t += model.step();
	}

	return 0;
}
