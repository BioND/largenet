/**
 * @file boost_largenet_multigraph.h
 * @date Dec 08, 2008
 * @author Gerd Zschaler
 * boost::graph interface for lnet::MultiNetwork class.
 */

#ifndef BOOST_LARGENET_MULTIGRAPH_H_
#define BOOST_LARGENET_MULTIGRAPH_H_

#include "../base/types.h"
#include "../MultiNetwork.h"
#include "../Network.h"

//#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <utility>

namespace boost
{

struct largenet_multigraph_traversal_category: public virtual bidirectional_graph_tag,
		public virtual adjacency_graph_tag,
		public virtual vertex_list_graph_tag,
		public virtual edge_list_graph_tag
{
};

template<>
struct graph_traits<lnet::MultiNetwork>
{
	typedef lnet::node_id_t vertex_descriptor;
	typedef lnet::link_id_t edge_descriptor;

	typedef lnet::MultiNetwork::NeighborLinkIterator out_edge_iterator;
	typedef lnet::MultiNetwork::NeighborLinkIterator in_edge_iterator;
	typedef lnet::MultiNetwork::LinkIterator edge_iterator;
	typedef lnet::MultiNetwork::NodeIterator vertex_iterator;

	typedef largenet_multigraph_traversal_category traversal_category;
	typedef undirected_tag directed_category;
	typedef allow_parallel_edge_tag edge_parallel_category;
	typedef lnet::id_size_t vertices_size_type;
	typedef lnet::id_size_t edges_size_type;
	typedef lnet::id_size_t degree_size_type;

	typedef lnet::node_state_t vertex_property_type;
	typedef lnet::link_state_t edge_property_type;
};

/*
 * boost::VertexListGraph concept
 */
graph_traits<lnet::MultiNetwork>::vertex_descriptor add_vertex(
		lnet::MultiNetwork& g);

std::pair<graph_traits<lnet::MultiNetwork>::edge_descriptor, bool>
		add_edge(graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
				graph_traits<lnet::MultiNetwork>::vertex_descriptor v,
				lnet::MultiNetwork& g);

void remove_edge(graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		graph_traits<lnet::MultiNetwork>::vertex_descriptor v,
		lnet::MultiNetwork& g);

void remove_edge(graph_traits<lnet::MultiNetwork>::edge_descriptor e,
		lnet::MultiNetwork& g);

void remove_edge(graph_traits<lnet::MultiNetwork>::edge_iterator it,
		lnet::MultiNetwork& g);

graph_traits<lnet::MultiNetwork>::vertex_descriptor add_vertex(
		lnet::MultiNetwork& g);

void clear_vertex(graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		lnet::MultiNetwork& g);

void remove_vertex(graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		lnet::MultiNetwork& g);

std::pair<graph_traits<lnet::MultiNetwork>::edge_descriptor, bool> add_edge(
		graph_traits<lnet::MultiNetwork>::vertex_descriptor u, graph_traits<
				lnet::MultiNetwork>::vertex_descriptor v, graph_traits<
				lnet::MultiNetwork>::edge_property_type ep,
		lnet::MultiNetwork& g);

graph_traits<lnet::MultiNetwork>::vertex_descriptor add_vertex(graph_traits<
		lnet::MultiNetwork>::vertex_property_type vp, lnet::MultiNetwork& g);

inline graph_traits<lnet::MultiNetwork>::vertex_descriptor null_vertex(
		const lnet::MultiNetwork& g)
{
	return g.numberOfNodes(); //FIXME this is a bad hack!
}

inline graph_traits<lnet::MultiNetwork>::vertex_descriptor source(graph_traits<
		lnet::MultiNetwork>::edge_descriptor e, const lnet::MultiNetwork& g)
{
	return g.source(e);
}

inline graph_traits<lnet::MultiNetwork>::vertex_descriptor target(graph_traits<
		lnet::MultiNetwork>::edge_descriptor e, const lnet::MultiNetwork& g)
{
	return g.target(e);
}

inline std::pair<graph_traits<lnet::MultiNetwork>::out_edge_iterator,
		graph_traits<lnet::MultiNetwork>::out_edge_iterator> out_edges(
		graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		const lnet::MultiNetwork& g)
{
	return g.neighborLinks(u);
}

inline std::pair<graph_traits<lnet::MultiNetwork>::in_edge_iterator,
		graph_traits<lnet::MultiNetwork>::in_edge_iterator> in_edges(
		graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		const lnet::MultiNetwork& g)
{
	return g.neighborLinks(u);
}

inline graph_traits<lnet::MultiNetwork>::degree_size_type in_degree(
		graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		const lnet::MultiNetwork& g)
{
	return g.degree(u);
}

inline graph_traits<lnet::MultiNetwork>::degree_size_type out_degree(
		graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		const lnet::MultiNetwork& g)
{
	return g.degree(u);
}

inline graph_traits<lnet::MultiNetwork>::degree_size_type degree(graph_traits<
		lnet::MultiNetwork>::vertex_descriptor u, const lnet::MultiNetwork& g)
{
	return g.degree(u);
}

inline graph_traits<lnet::MultiNetwork>::vertices_size_type num_vertices(
		const lnet::MultiNetwork& g)
{
	return g.numberOfNodes();
}

inline std::pair<graph_traits<lnet::MultiNetwork>::vertex_iterator,
		graph_traits<lnet::MultiNetwork>::vertex_iterator> vertices(
		const lnet::MultiNetwork& g)
{
	return g.nodes();
}

inline std::pair<graph_traits<lnet::MultiNetwork>::edge_iterator, graph_traits<
		lnet::MultiNetwork>::edge_iterator> edges(const lnet::MultiNetwork& g)
{
	return g.links();
}

inline graph_traits<lnet::MultiNetwork>::edges_size_type num_edges(
		const lnet::MultiNetwork& g)
{
	return g.numberOfLinks();
}

template<class Predicate>
void remove_edge_if(Predicate p, lnet::MultiNetwork& g)
{
	lnet::MultiNetwork::LinkIteratorRange iters = g.links();
	for (lnet::MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
	{
		if (p(*it))
			g.removeLink(*it); // repo::CategorizedRepository::IndexIterator is not invalidated by deletion of items (really?)
	}
}

template<class Predicate>
void remove_out_edge_if(graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		Predicate p, lnet::MultiNetwork& g)
{
	lnet::MultiNetwork::NeighborLinkIteratorRange iters = g.neighborLinks(u);
	for (lnet::MultiNetwork::NeighborLinkIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		if (p(*it))
			g.removeLink(*it);
	}
}

template<class Predicate>
void remove_in_edge_if(graph_traits<lnet::MultiNetwork>::vertex_descriptor u,
		Predicate p, lnet::MultiNetwork& g)
{
	lnet::MultiNetwork::NeighborLinkIteratorRange iters = g.neighborLinks(u);
	for (lnet::MultiNetwork::NeighborLinkIterator& it = iters.first; it
			!= iters.second; ++it)
	{
		if (p(*it))
			g.removeLink(*it);
	}
}

/*
 * boost::PropertyGraph concept
 */

/**
 * Vertex ID property map implementation.
 */
class largenet_multigraph_node_id_property_map: public put_get_helper<
		lnet::node_id_t, largenet_multigraph_node_id_property_map>
{
public:
	typedef graph_traits<lnet::MultiNetwork>::vertex_descriptor key_type;
	typedef lnet::node_id_t value_type;
	typedef lnet::node_id_t reference;
	typedef readable_property_map_tag category;

	value_type operator[](key_type e) const
	{
		return e;
	}
};

/**
 * Vertex ID property map.
 */
template<>
struct property_map<lnet::MultiNetwork, vertex_index_t>
{
	typedef largenet_multigraph_node_id_property_map type;
	typedef largenet_multigraph_node_id_property_map const_type;
};

inline property_map<lnet::MultiNetwork, vertex_index_t>::const_type get(
		vertex_index_t, const lnet::MultiNetwork& g)
{
	return largenet_multigraph_node_id_property_map();
}

/*
 * Vertex/edge states.
 */

/**
 * Vertex state property map implementation.
 */
class largenet_multigraph_vertex_state_property_map: public put_get_helper<
		graph_traits<lnet::MultiNetwork>::vertex_descriptor,
		largenet_multigraph_vertex_state_property_map>
{
public:
	typedef graph_traits<lnet::MultiNetwork>::vertex_descriptor key_type;
	typedef lnet::node_state_t value_type;
	typedef lnet::node_state_t reference;
	typedef readable_property_map_tag category;

	largenet_multigraph_vertex_state_property_map(const lnet::MultiNetwork& net) :
		net_(&net)
	{
	}

	value_type operator[](key_type e) const
	{
		return net_->nodeState(e);
	}

private:
	const lnet::MultiNetwork* net_;
};

/**
 * Edge state property map implementation.
 */
class largenet_multigraph_edge_state_property_map: public put_get_helper<
		graph_traits<lnet::MultiNetwork>::edge_descriptor,
		largenet_multigraph_edge_state_property_map>
{
public:
	typedef graph_traits<lnet::MultiNetwork>::edge_descriptor key_type;
	typedef lnet::link_state_t value_type;
	typedef lnet::link_state_t reference;
	typedef readable_property_map_tag category;

	largenet_multigraph_edge_state_property_map(const lnet::MultiNetwork& net) :
		net_(&net)
	{
	}

	value_type operator[](key_type e) const
	{
		return net_->linkState(e);
	}

private:
	const lnet::MultiNetwork* net_;
};

struct vertex_state_t
{
	typedef vertex_property_tag kind;
};

struct edge_state_t
{
	typedef edge_property_tag kind;
};

/**
 * Vertex state property map.
 */
template<>
struct property_map<lnet::MultiNetwork, vertex_state_t>
{
	typedef largenet_multigraph_vertex_state_property_map type;
	typedef largenet_multigraph_vertex_state_property_map const_type;
};

/**
 * Edge state property map.
 */
template<>
struct property_map<lnet::MultiNetwork, edge_state_t>
{
	typedef largenet_multigraph_edge_state_property_map type;
	typedef largenet_multigraph_edge_state_property_map const_type;
};

inline property_map<lnet::MultiNetwork, vertex_state_t>::const_type get(
		vertex_state_t, const lnet::MultiNetwork& g)
{
	return largenet_multigraph_vertex_state_property_map(g);
}

inline property_map<lnet::MultiNetwork, edge_state_t>::const_type get(
		edge_state_t, const lnet::MultiNetwork& g)
{
	return largenet_multigraph_edge_state_property_map(g);
}

} // namespace boost


#endif /* BOOST_LARGENET_MULTIGRAPH_H_ */
