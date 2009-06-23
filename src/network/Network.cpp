#include "Network.h"

namespace lnet
{

Network::Network() :
	MultiNetwork()
{
}

Network::Network(const Network& net) :
	MultiNetwork(net)
{
}

Network::Network(const id_size_t nNodes, const id_size_t nLinks,
		const id_size_t nNodeStates, LinkStateCalculator* lsCalc) :
	MultiNetwork(nNodes, nLinks, nNodeStates, lsCalc)
{
}

Network::~Network()
{
}

link_id_t Network::doAddLink(const node_id_t source, const node_id_t target)
{
	std::pair<bool, link_id_t> linkExists = isLink(source, target);
	link_id_t l;
	if (!linkExists.first)
	{
		l = MultiNetwork::doAddLink(source, target);
	}
	else
	{
		l = linkExists.second;
	}
	return l;
}

bool Network::doChangeLink(const link_id_t l, const node_id_t source,
		const node_id_t target)
{
	if (isLink(source, target).first)
		return false;

	return MultiNetwork::doChangeLink(l, source, target);
}

}
