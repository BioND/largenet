/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
#include "GraphvizIn.h"
#include "../base/BasicNetwork.h"
#include <algorithm>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include "../boost/boost_largenet_multigraph.h"
#include <boost/graph/graphviz.hpp>
#include <boost/version.hpp>
#include <cassert>

namespace lnet
{

namespace io
{

using namespace boost;

GraphvizIn::GraphvizIn() :
	NetIn()
{
}

GraphvizIn::~GraphvizIn()
{
}

#if (BOOST_VERSION < 103700)

//=========================================================================
// A property map that always returns a reference to the same object.
// (from boost 1.37.0)
template<typename KeyType, typename ValueType>
class ref_property_map: public boost::put_get_helper<ValueType&,
		ref_property_map<KeyType, ValueType> >
{
	ValueType* value;
public:
	typedef KeyType key_type;
	typedef ValueType value_type;
	typedef ValueType& reference;
	typedef lvalue_property_map_tag category;
	ref_property_map(ValueType& v) :
		value(&v)
	{
	}
	ValueType& operator[](key_type const&) const
	{
		return *value;
	}
};
#endif

bool GraphvizIn::get(std::istream& in, BasicNetwork& net)
{
	// TODO implement me.
	/*
	if (!in)
		return false;

	// Vertex properties
	typedef property<vertex_name_t, node_id_t, property<vertex_state_t,
			node_state_t> > vertex_p;
	// Edge properties
	typedef property<edge_state_t, link_state_t> edge_p;

	// adjacency_list-based type
	typedef adjacency_list<vecS, vecS, undirectedS, vertex_p, edge_p> graph_t;

	// Construct an empty graph and prepare the dynamic_property_maps.
	graph_t graph(0);
	dynamic_properties dp;

	property_map<graph_t, vertex_state_t>::type nodeState(boost::get(
			vertex_state_t(), graph));
	property_map<graph_t, edge_state_t>::type edgeState(boost::get(
			edge_state_t(), graph));

	if (readStates_)
	{
		dp.property("label", nodeState);
		dp.property("label", edgeState);
	}

	property_map<graph_t, vertex_name_t>::type nodeID(boost::get(vertex_name,
			graph));
	dp.property("node_id", nodeID);

	bool retval = read_graphviz(in, graph, dp);

	node_state_size_t nStates = 1;
	link_state_size_t lStates = 1;
	if (readStates_)
	{
		// TODO get number of node/link states from maximum label value
	}

	net.reset(num_vertices(graph), num_edges(graph), nStates, lStates);

	std::pair<graph_traits<graph_t>::edge_iterator,
			graph_traits<graph_t>::edge_iterator> iters = edges(graph);
	if (readStates_)
	{
		for (graph_traits<graph_t>::edge_iterator it = iters.first; it
				!= iters.second; ++it)
		{
			node_id_t a = source(*it, graph), b = target(*it, graph);
			net.setNodeState(a, nodeState[a]);
			net.setNodeState(b, nodeState[b]);
			net.addLink(a, b, edgeState[*it]);
		}
	}
	else
	{
		for (graph_traits<graph_t>::edge_iterator it = iters.first; it
				!= iters.second; ++it)
		{
			node_id_t a = source(*it, graph), b = target(*it, graph);
			net.addLink(a, b);
		}
	}
	return retval;
	*/
	return true;
}

}

}
