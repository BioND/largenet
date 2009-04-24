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
	typedef std::pair<LinkIDIterator, LinkIDIterator> iter_range; ///< Iterator range type.

	/**
	 * Basic constructor. Creates an isolated node object.
	 */
	Node();
	/*
	 * Constructor. Creates an isolated node with initial state @p state.
	 * @param state Initial state of the node.
	 */
//	explicit Node(node_state_t state);
	/**
	 * Destructor. Does nothing.
	 */
	~Node();

	/**
	 * Return the degree (number of adjacent nodes) of the node.
	 * @return Degree of the node.
	 */
	id_size_t degree() const;

	/*
	 * Get node state.
	 * @return Node state.
	 */
//	node_state_t state() const;
	/*
	 * Set node state.
	 * @param state New state of the node.
	 */
//	void setState(node_state_t state);

	/**
	 * Add link with ID @p l to node. The link ID is inserted in the node's link
	 * list, thus increasing the node's degree by one.
	 * @param l %Link ID to add.
	 */
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
	bool hasLink(link_id_t l) const;

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
	iter_range links();

private:
	LinkSet links_; ///< Multiset of links to neighbors
//	node_state_t state_; ///< State of node.
};

inline id_size_t Node::degree() const
{
	return links_.size();
}

//inline node_state_t Node::state() const
//{
//	return state_;
//}


inline Node::iter_range Node::links()
{
	return std::make_pair(links_.begin(), links_.end());
}

//inline void Node::setState(const node_state_t state)
//{
//	state_ = state;
//}

}

#endif /*NODE_H_*/
