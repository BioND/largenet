/**
 * @file traits.h
 * @date 13.05.2009
 * @author gerd
 */

#ifndef TRAITS_H_
#define TRAITS_H_

namespace lnet
{

struct allow_parallel_edge_tag {};
struct disallow_parallel_edge_tag {};

struct count_triples_tag {};
struct no_count_triples_tag {};

template<class _Network>
struct network_traits
{
	typedef	typename _Network::NodeType NodeType;
	typedef typename _Network::LinkType LinkType;
	typedef typename _Network::NodeIterator NodeIterator; ///< %Node ID iterator type.
	typedef typename _Network::NodeStateIterator NodeStateIterator; ///< %Node ID in state iterator type.
	typedef typename _Network::NodeIteratorRange NodeIteratorRange; ///< %Node ID iterator range type.
	typedef typename _Network::NodeStateIteratorRange NodeStateIteratorRange; ///< %Node ID in state iterator range type.
	typedef typename _Network::LinkIterator LinkIterator; ///< %Link ID iterator type.
	typedef typename _Network::LinkStateIterator LinkStateIterator; ///< %Link ID in state iterator type.
	typedef typename _Network::LinkIteratorRange LinkIteratorRange; ///< %Link ID iterator range type.
	typedef typename _Network::LinkStateIteratorRange LinkStateIteratorRange; ///< %Link ID in state iterator range type.

	typedef typename _Network::NeighborLinkIterator NeighborLinkIterator; ///< %Link ID iterator type for neighboring links of a given node.
	typedef typename _Network::NeighborLinkIteratorRange NeighborLinkIteratorRange; ///< %Link ID iterator range type for neighboring links of a given node.
	typedef typename _Network::NeighborIterator NeighborIterator;
	typedef typename _Network::NeighborIteratorRange NeighborIteratorRange; ///< Neighbor iterator range type.

	// boost inspired stuff
	typedef typename _Network::edge_parallel_category edge_parallel_category;
	typedef typename _Network::triple_counting_category triple_counting_category;
};

template<class _TNetwork>
struct triple_network_traits: public network_traits<_TNetwork>
{
	typedef typename _TNetwork::TripleType TripleType;
	typedef typename _TNetwork::TripleIterator TripleIterator;
	typedef typename _TNetwork::TripleStateIterator TripleStateIterator;
	typedef typename _TNetwork::TripleIteratorRange TripleIteratorRange;
	typedef typename _TNetwork::TripleStateIteratorRange TripleStateIteratorRange;
	typedef typename _TNetwork::NeighborTripleIterator NeighborTripleIterator;
	typedef typename _TNetwork::NeighborTripleIterartorRange NeighborTripleIteratorRange;
	typedef count_triples_tag triple_counting_category;
};

namespace detail
{
	inline bool allows_parallel(allow_parallel_edge_tag) { return true; }
	inline bool allows_parallel(disallow_parallel_edge_tag) { return false; }
	inline bool counts_triples(count_triples_tag) { return true; }
	inline bool counts_triples(no_count_triples_tag) { return false; }
}

template <class _Network>
bool allows_parallel_edges(const _Network&) {
  typedef typename network_traits<_Network>::edge_parallel_category Cat;
  return detail::allows_parallel(Cat());
}

template <class _Network>
bool counts_triples(const _Network&)
{
  typedef typename network_traits<_Network>::triple_counting_category Cat;
  return detail::counts_triples(Cat());
}


}

#endif /* TRAITS_H_ */
