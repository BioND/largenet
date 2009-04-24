#include "Network.h"

namespace lnet
{

Network::Network() :
	MultiNetwork()
{
}

Network::Network(const id_size_t nNodes, const id_size_t nLinks,
		const id_size_t nNodeStates, const id_size_t nLinkStates) :
	MultiNetwork(nNodes, nLinks, nNodeStates, nLinkStates)
{
}

Network::~Network()
{
}

link_id_t Network::addLink(const node_id_t source, const node_id_t target)
{
	return addLink(source, target, 0);
}

link_id_t Network::addLink(const node_id_t source, const node_id_t target, const link_state_t s)
{
	std::pair<bool, link_id_t> linkExists = isLink(source, target);
	link_id_t l;
	if (!linkExists.first)
	{
		l = MultiNetwork::addLink(source, target, s);
	}
	else
	{
		l = linkExists.second;
		setLinkState(l, s);
	}
	return l;
}

bool Network::changeLink(const link_id_t l, const node_id_t source, const node_id_t target, const link_state_t s)
{
	if (isLink(source, target).first)
		return false;

	return MultiNetwork::changeLink(l, source, target, s);
}

}
