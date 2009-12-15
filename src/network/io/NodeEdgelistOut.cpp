/*
 * NodeEdgelistOut.cpp
 *
 *  Created on: 16.04.2009
 *      Author: gerd
 */

#include "NodeEdgelistOut.h"
#include "iodefs.h"

using namespace std;

namespace lnet
{

namespace io
{

NodeEdgelistOut::NodeEdgelistOut() :
	NetOut()
{
}

NodeEdgelistOut::~NodeEdgelistOut()
{
}

bool NodeEdgelistOut::doPut(std::ostream& out, const BasicNetwork& net) const
{
	if (!out)
		return false;

	const char sep = '\t';

	// write number of nodes and edges
	out << net.numberOfNodes() << sep << net.numberOfLinks();

	if (writeStates_)
		out << sep << net.numberOfNodeStates() << sep
				<< net.numberOfLinkStates();
	out << "\n";

	// write separator
	out << NODE_LIST << "\n";

	// write node list:
	BasicNetwork::NodeIteratorRange niters = net.nodes();
	if (writeStates_)
	{
		// ID  state
		for (BasicNetwork::NodeIterator& nit = niters.first; nit
				!= niters.second; ++nit)
		{
			out << *nit << sep << net.nodeState(*nit) << "\n";
		}

		// write separator
		out << LINK_LIST << "\n";

		// write link list:
		// ID  source  target  state
		BasicNetwork::LinkIteratorRange liters = net.links();
		for (BasicNetwork::LinkIterator& lit = liters.first; lit
				!= liters.second; ++lit)
		{
			out << *lit << sep << net.source(*lit) << sep << net.target(*lit)
					<< sep << net.linkState(*lit) << "\n";
		}
	}
	else
	{
		// ID
		for (BasicNetwork::NodeIterator& nit = niters.first; nit
				!= niters.second; ++nit)
		{
			out << *nit << "\n";
		}

		// write separator
		out << LINK_LIST << "\n";

		// write link list:
		// ID  source  target  state
		BasicNetwork::LinkIteratorRange liters = net.links();
		for (BasicNetwork::LinkIterator& lit = liters.first; lit
				!= liters.second; ++lit)
		{
			out << *lit << sep << net.source(*lit) << sep << net.target(*lit)
					<< "\n";
		}

	}
	return true;
}

}

}
