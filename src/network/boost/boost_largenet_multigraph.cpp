#include "boost_largenet_multigraph.h"

using namespace lnet;

namespace boost
{

/*
 * boost::MutableGraph concept
 */
std::pair<graph_traits<MultiNetwork>::edge_descriptor, bool> add_edge(
		graph_traits<MultiNetwork>::vertex_descriptor u, graph_traits<
				MultiNetwork>::vertex_descriptor v, MultiNetwork& g)
{
	// this must be changed if parallel edges are disallowed
	return std::make_pair(g.addLink(u, v), true);
}

void remove_edge(graph_traits<MultiNetwork>::vertex_descriptor u, graph_traits<
		MultiNetwork>::vertex_descriptor v, MultiNetwork& g)
{
	std::pair<bool, link_id_t> p = g.isLink(u, v);
	if (p.first)
		g.removeLink(p.second);
}

void remove_edge(graph_traits<MultiNetwork>::edge_descriptor e, MultiNetwork& g)
{
	g.removeLink(e);
}

void remove_edge(graph_traits<MultiNetwork>::edge_iterator it, MultiNetwork& g)
{
	g.removeLink(*it);
}

graph_traits<MultiNetwork>::vertex_descriptor add_vertex(MultiNetwork& g)
{
	return g.addNode();
}

void clear_vertex(graph_traits<MultiNetwork>::vertex_descriptor u,
		MultiNetwork& g)
{
	MultiNetwork::NeighborLinkIteratorRange iters = g.neighborLinks(u);
	for (MultiNetwork::NeighborLinkIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		g.removeLink(*it);
	}
}

void remove_vertex(graph_traits<MultiNetwork>::vertex_descriptor u,
		MultiNetwork& g)
{
	g.removeNode(u);
}


/*
 * boost::MutablePropertyGraph concept
 */
/*std::pair<graph_traits<MultiNetwork>::edge_descriptor, bool> add_edge(
		graph_traits<MultiNetwork>::vertex_descriptor u, graph_traits<
				MultiNetwork>::vertex_descriptor v,
		graph_traits<MultiNetwork>::edge_property_type ep, MultiNetwork& g)
{
	// this must be changed if parallel edges are disallowed
	return std::make_pair(g.addLink(u, v, ep), true);
}

graph_traits<MultiNetwork>::vertex_descriptor add_vertex(graph_traits<
		MultiNetwork>::vertex_property_type vp, MultiNetwork& g)
{
	return g.addNode(vp);
}
*/
}
