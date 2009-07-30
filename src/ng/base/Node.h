/**
 * @file Node.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Node class.
 */
#ifndef NODE_H_
#define NODE_H_

#include "traits.h"
#include <set>
#include <utility>

namespace lnet
{

namespace detail
{

template<class directed_category, typename edge_descriptor>
class NodeData;

template<typename edge_descriptor>
class NodeData<directed_tag, edge_descriptor>
{
public:
	typedef std::set<edge_descriptor> edge_set;
	typedef typename edge_set::iterator edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;
	typedef typename edge_set::size_type degree_size_type;

	void addInEdge(edge_descriptor l);
	void addOutEdge(edge_descriptor l);
	degree_size_type inDegree() const;
	degree_size_type outDegree() const;
	degree_size_type degree() const;
	/**
	 * Remove link with ID @p l from node. The link ID is removed from the node's
	 * link list, thus decreasing the node's degree by one.
	 * @param l %Link ID to remove.
	 */
	void removeEdge(edge_descriptor l);
	/**
	 * Return true if link ID @p l is in the node's link list.
	 * @param l %Link ID to look for.
	 * @return True if the link ID is found in the node's link list.
	 */
	bool partOfEdge(edge_descriptor l) const;

	/**
	 * Isolate node from neighbors by clearing its link list.
	 * Note that this does only affect the node, and not its neighbors.
	 */
	void clear();

	/**
	 * Return iterator range for all IDs of the node's links.
	 * @return std::pair of LinkIDIterators, the first pointing to the first
	 * link ID in the node's link list and the second pointing past-the-end
	 */
	edge_iterator_range inEdges();
	edge_iterator_range outEdges();
protected:
	edge_set inEdges_, outEdges_;
};

template<typename edge_descriptor>
class NodeData<undirected_tag, edge_descriptor>
{
public:
	typedef std::set<edge_descriptor> edge_set;
	typedef typename edge_set::iterator edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;
	typedef typename edge_set::size_type degree_size_type;

	void addEdge(edge_descriptor l);
	degree_size_type degree() const;
	/**
	 * Remove link with ID @p l from node. The link ID is removed from the node's
	 * link list, thus decreasing the node's degree by one.
	 * @param l %Link ID to remove.
	 */
	void removeEdge(edge_descriptor l);
	/**
	 * Return true if link ID @p l is in the node's link list.
	 * @param l %Link ID to look for.
	 * @return True if the link ID is found in the node's link list.
	 */
	bool partOfEdge(edge_descriptor l) const;

	/**
	 * Isolate node from neighbors by clearing its link list.
	 * Note that this does only affect the node, and not its neighbors.
	 */
	void clear();

	/**
	 * Return iterator range for all IDs of the node's links.
	 * @return std::pair of LinkIDIterators, the first pointing to the first
	 * link ID in the node's link list and the second pointing past-the-end
	 */
	edge_iterator_range edges();

protected:
	edge_set edges_;
};

template<class directed_category, typename edge_descriptor>
class NodeImpl: public NodeData<directed_category, edge_descriptor>
{
};

}

/**
 * Class representing one network node.
 *
 * A %Node keeps a list of the IDs of adjacent links.
 */
template<class G>
class Node: public detail::NodeImpl<
		typename graph_traits<G>::directed_category,
		typename graph_traits<G>::edge_descriptor>
{
};

namespace detail
{

template<typename edge_descriptor>
inline void NodeData<undirected_tag, edge_descriptor>::addEdge(
		const edge_descriptor l)
{
	edges_.insert(l);
}

template<typename edge_descriptor>
inline void NodeData<directed_tag, edge_descriptor>::addInEdge(
		const edge_descriptor l)
{
	inEdges_.insert(l);
}

template<typename edge_descriptor>
inline void NodeData<directed_tag, edge_descriptor>::addOutEdge(
		const edge_descriptor l)
{
	outEdges_.insert(l);
}

template<typename edge_descriptor>
inline void NodeData<undirected_tag, edge_descriptor>::removeEdge(
		const edge_descriptor l)
{
	edge_iterator it = edges_.find(l);
	if (it != edges_.end())
		edges_.erase(it);
}

template<typename edge_descriptor>
inline void NodeData<directed_tag, edge_descriptor>::removeEdge(
		const edge_descriptor l)
{
	edge_iterator it = inEdges_.find(l);
	if (it != inEdges_.end())
		inEdges_.erase(it);
	it = outEdges_.find(l);
	if (it != outEdges_.end())
		outEdges_.erase(it);
}

template<typename edge_descriptor>
inline typename NodeData<undirected_tag, edge_descriptor>::degree_size_type NodeData<
		undirected_tag, edge_descriptor>::degree() const
{
	return edges_.size();
}

template<typename edge_descriptor>
inline typename NodeData<directed_tag, edge_descriptor>::degree_size_type NodeData<
		directed_tag, edge_descriptor>::degree() const
{
	return inEdges_.size() + outEdges_.size();
}

template<typename edge_descriptor>
inline typename NodeData<directed_tag, edge_descriptor>::degree_size_type NodeData<
		directed_tag, edge_descriptor>::inDegree() const
{
	return inEdges_.size();
}

template<typename edge_descriptor>
inline typename NodeData<directed_tag, edge_descriptor>::degree_size_type NodeData<
		directed_tag, edge_descriptor>::outDegree() const
{
	return outEdges_.size();
}

template<typename edge_descriptor>
inline typename NodeData<undirected_tag, edge_descriptor>::edge_iterator_range NodeData<
		undirected_tag, edge_descriptor>::edges()
{
	return std::make_pair(edges_.begin(), edges_.end());
}

template<typename edge_descriptor>
inline typename NodeData<directed_tag, edge_descriptor>::edge_iterator_range NodeData<
		directed_tag, edge_descriptor>::inEdges()
{
	return std::make_pair(inEdges_.begin(), inEdges_.end());
}

template<typename edge_descriptor>
inline typename NodeData<directed_tag, edge_descriptor>::edge_iterator_range NodeData<
		directed_tag, edge_descriptor>::outEdges()
{
	return std::make_pair(outEdges_.begin(), outEdges_.end());
}

template<typename edge_descriptor>
inline void NodeData<undirected_tag, edge_descriptor>::clear()
{
	edges_.clear();
}

template<typename edge_descriptor>
inline void NodeData<directed_tag, edge_descriptor>::clear()
{
	inEdges_.clear();
	outEdges_.clear();
}

template<typename edge_descriptor>
inline bool NodeData<undirected_tag, edge_descriptor>::partOfEdge(
		const edge_descriptor l) const
{
	return edges_.find(l) != edges_.end();
}

template<typename edge_descriptor>
inline bool NodeData<directed_tag, edge_descriptor>::partOfEdge(
		const edge_descriptor l) const
{
	return (inEdges_.find(l) != inEdges_.end()) || (outEdges_.find(l)
			!= outEdges_.end());
}

}
}
#endif /*NODE_H_*/
