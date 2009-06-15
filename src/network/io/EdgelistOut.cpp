#include "EdgelistOut.h"
#include "../base/types.h"

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

bool EdgelistOut::doPut(std::ostream& out, const MultiNetwork& net) const
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

	MultiNetwork::LinkIteratorRange iters = net.links();
	if (writeStates_)
	{
		for (MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
		{
			out << net.source(*it) << sep << net.target(*it) << sep
					<< net.nodeState(net.source(*it)) << sep << net.nodeState(
					net.target(*it)) << sep << net.linkState(*it) << "\n";
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
