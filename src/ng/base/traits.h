/**
 * @file traits.h
 * @date 29.07.2009
 * @author gerd
 */

#ifndef TRAITS_H_
#define TRAITS_H_

namespace lnet
{

struct directed_tag {};
struct undirected_tag {};

struct allow_parallel_edge_tag {};
struct disallow_parallel_edge_tag {};

template<class G> struct graph_traits
{
	typedef typename G::vertex_descriptor vertex_descriptor;
	typedef typename G::edge_descriptor edge_descriptor;
	typedef typename G::directed_category directed_category;
	typedef typename G::edge_parallel_category edge_parallel_category;

	typedef typename G::degree_size_type degree_size_type;
	typedef typename G::vertices_size_type vertices_size_type;
	typedef typename G::edges_size_type edges_size_type;
};

}

#endif /* TRAITS_H_ */
