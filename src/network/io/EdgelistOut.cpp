#include "EdgelistOut.h"
#include "../types.h"
#include "../Link.h"
#include "../MultiNetwork.h"

namespace lnet
{

namespace io
{

using namespace std;

EdgelistOut::EdgelistOut() :
	NetOut()
{
}

EdgelistOut::~EdgelistOut()
{
}

bool EdgelistOut::put(std::ostream& out, const MultiNetwork& net)
{
	if (!out)
		return false;

	const char sep = '\t';

	// write number of nodes and edges
	out << net.numberOfNodes() << sep << net.numberOfLinks() << sep
			<< net.numberOfNodeStates() << sep << net.numberOfLinkStates()
			<< "\n";

	MultiNetwork::LinkIteratorRange iters = net.links();
	if (writeStates_)
	{
		for (MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
		{
			out << net.source(*it) << sep << net.target(*it) << sep
					<< net.getNodeState(net.source(*it)) << sep
					<< net.getNodeState(net.target(*it)) << sep
					<< net.getLinkState(*it) << "\n";
		}
	}
	else
	{
		for (MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
		{
			out << net.source(*it) << sep << net.target(*it) << "\n";
		}
	}
	return true;
}

}

}
