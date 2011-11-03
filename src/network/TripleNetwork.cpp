/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file TripleNetwork.cpp
 * @date 13.05.2009
 * @author gerd
 */

#include "TripleNetwork.h"

namespace lnet
{

TripleNetwork::TripleNetwork() :
	TripleMultiNetwork()
{
}

TripleNetwork::TripleNetwork(const TripleNetwork& net) :
	TripleMultiNetwork(net)
{
}

TripleNetwork::TripleNetwork(const id_size_t nNodes, const id_size_t nLinks,
		const id_size_t nNodeStates, LinkStateCalculator* lsCalc,
		TripleStateCalculator* tsCalc) :
	TripleMultiNetwork(nNodes, nLinks, nNodeStates, lsCalc, tsCalc)
{
}

TripleNetwork::~TripleNetwork()
{
}

link_id_t TripleNetwork::doAddLink(const node_id_t source,
		const node_id_t target)
{
	std::pair<bool, link_id_t> linkExists = isLink(source, target);
	link_id_t l;
	if (!linkExists.first)
	{
		l = TripleMultiNetwork::doAddLink(source, target);
	}
	else
	{
		l = linkExists.second;
	}
	return l;
}

bool TripleNetwork::doChangeLink(const link_id_t l, const node_id_t source,
		const node_id_t target)
{
	if (isLink(source, target).first)
		return false;

	return TripleMultiNetwork::doChangeLink(l, source, target);
}

}
