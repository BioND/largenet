/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file Node.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Node class.
 */
#ifndef NODE_H_
#define NODE_H_

#include <largenet/base/types.h>
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
	bool inLink(link_id_t l) const;

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
	LinkIDIteratorRange links();

private:
	LinkSet links_; ///< Multiset of links to neighbors
};

inline Node::Node()
{
}

inline void Node::addLink(const link_id_t l)
{
	links_.insert(l);
}

inline void Node::removeLink(const link_id_t l)
{
	LinkSet::iterator it = links_.find(l);
	if (it != links_.end())
		links_.erase(it);
}

inline id_size_t Node::degree() const
{
	return links_.size();
}

inline Node::LinkIDIteratorRange Node::links()
{
	return std::make_pair(links_.begin(), links_.end());
}

inline void Node::clear()
{
	links_.clear();
}

inline bool Node::inLink(const link_id_t l) const
{
	return links_.find(l) != links_.end();
}

}

#endif /*NODE_H_*/
