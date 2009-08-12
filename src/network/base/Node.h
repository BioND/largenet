/**
 * @file Node.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Node class.
 */
#ifndef NODE_H_
#define NODE_H_

#include "types.h"
#include <set>
#include <utility>

namespace lnet
{

/**
 * Class representing one network node.
 *
 * A %Node keeps a list of the IDs of adjacent links.
 */
class Node
{
public:
	typedef std::multiset<link_id_t> LinkSet;
	typedef LinkSet::iterator LinkIDIterator; ///< %Link ID iterator type.
	typedef std::pair<LinkIDIterator, LinkIDIterator> LinkIDIteratorRange; ///< Iterator range type.

	/**
	 * Basic constructor. Creates an isolated node object.
	 */
	Node();

	/**
	 * Return the degree (number of adjacent nodes) of the node.
	 * @return Degree of the node.
	 */
	id_size_t degree() const;
	id_size_t inDegree() const;
	id_size_t outDegree() const;

	/**
	 * Add link with ID @p l to node. The link ID is inserted in the node's link
	 * list, thus increasing the node's degree by one.
	 * @param l %Link ID to add.
	 */
	void addInLink(link_id_t l);
	void addOutLink(link_id_t l);
	void addLink(link_id_t l);

	/**
	 * Remove link with ID @p l from node. The link ID is removed from the node's
	 * link list, thus decreasing the node's degree by one.
	 * @param l %Link ID to remove.
	 */
	void removeLink(link_id_t l);
	/**
	 * Return true if link ID @p l is in the node's link list.
	 * @param l %Link ID to look for.
	 * @return True if the link ID is found in the node's link list.
	 */
	bool inLink(link_id_t l) const;
	bool hasInLink(link_id_t l) const;
	bool hasOutLink(link_id_t l) const;

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
	LinkIDIteratorRange inLinks();
	LinkIDIteratorRange outLinks();

private:
	LinkSet outLinks_, inLinks_; ///< Multiset of links to neighbors
};

inline Node::Node()
{
}

inline void Node::addOutLink(const link_id_t l)
{
	outLinks_.insert(l);
}

inline void Node::addInLink(const link_id_t l)
{
	inLinks_.insert(l);
}

inline void Node::addLink(const link_id_t l)
{
	addOutLink(l);
}

inline void Node::removeLink(const link_id_t l)
{
	LinkSet::iterator it = outLinks_.find(l);
	if (it != outLinks_.end())
		outLinks_.erase(it);
	it = inLinks_.find(l);
	if (it != inLinks_.end())
		inLinks_.erase(it);
}

inline id_size_t Node::degree() const
{
	return outDegree() + inDegree();
}

inline id_size_t Node::inDegree() const
{
	return inLinks_.size();
}

inline id_size_t Node::outDegree() const
{
	return outLinks_.size();
}

inline Node::LinkIDIteratorRange Node::inLinks()
{
	return std::make_pair(inLinks_.begin(), inLinks_.end());
}

inline Node::LinkIDIteratorRange Node::outLinks()
{
	return std::make_pair(outLinks_.begin(), outLinks_.end());
}

inline void Node::clear()
{
	inLinks_.clear();
	outLinks_.clear();
}

inline bool Node::hasInLink(const link_id_t l) const
{
	return inLinks_.find(l) != inLinks_.end();
}

inline bool Node::hasOutLink(const link_id_t l) const
{
	return outLinks_.find(l) != outLinks_.end();
}

inline bool Node::inLink(const link_id_t l) const
{
	return hasOutLink(l) || hasInLink(l);
}

}

#endif /*NODE_H_*/
