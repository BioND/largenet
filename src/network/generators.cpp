/**
 * @file generators.cpp
 * @date 08.06.2009
 * @author gerd
 */

#include "generators.h"
#include "base/BasicNetwork.h"

namespace lnet
{

namespace generators
{

void erdosRenyiNetwork(BasicNetwork& net, const id_size_t nLinks,
		const bool selfLoops)
{
	id_size_t n = net.numberOfNodes();
	if (n < 1)
		return;
	assert(nLinks <= n * (n - 1) / 2);
	net.removeAllLinks();
	while (net.numberOfLinks() < nLinks)
	{
		std::pair<bool, node_id_t> a = net.randomNode(), b = net.randomNode();
		if (!selfLoops && (a.second == b.second))
			continue;
		net.addLink(a.second, b.second);
	}
}

}
}
