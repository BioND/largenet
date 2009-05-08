/**
 * @file MultiNetwork.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::MultiNetwork class.
 */
#ifndef MULTINETWORK_H_
#define MULTINETWORK_H_

#include "types.h"
#include "Node.h"
#include "Link.h"
#include "../repo/CategorizedRepository.h"
#include <cassert>
#include <utility> // for std::pair

namespace lnet
{

/**
 * A class representing a network of Nodes connected by Links. Parallel edges (multiple
 * links connecting the same nodes) and self-loops are allowed. %Nodes and links can have
 * different states that are automatically kept consistent using a LinkStateCalculator object.
 *
 * @todo We need some network class hierarchy representing directed/undirected graphs
 * 		 where parallel edges may be (dis)allowed, and self-loops, too. See boost::graph.
 */
class MultiNetwork
{
protected:
	// member types
	typedef repo::CategorizedRepository<Node*> NodeRepo; ///< CategorizedRepository of Node pointers
	typedef repo::CategorizedRepository<Link*> LinkRepo; ///< CategorizedRepository of Link pointers

public:
	// member types
	typedef NodeRepo::IndexIterator NodeIterator; ///< %Node ID iterator type.
	typedef NodeRepo::CategoryIterator NodeStateIterator; ///< %Node ID in state iterator type.
	typedef NodeRepo::IndexIteratorRange NodeIteratorRange; ///< %Node ID iterator range type.
	typedef NodeRepo::CategoryIteratorRange NodeStateIteratorRange; ///< %Node ID in state iterator range type.
	typedef LinkRepo::IndexIterator LinkIterator; ///< %Link ID iterator type.
	typedef LinkRepo::CategoryIterator LinkStateIterator; ///< %Link ID in state iterator type.
	typedef LinkRepo::IndexIteratorRange LinkIteratorRange; ///< %Link ID iterator range type.
	typedef LinkRepo::CategoryIteratorRange LinkStateIteratorRange; ///< %Link ID in state iterator range type.

	typedef Node::LinkIDIterator NeighborLinkIterator; ///< %Link ID iterator type for neighboring links of a given node.
	typedef Node::iter_range NeighborLinkIteratorRange; ///< %Link ID iterator range type for neighboring links of a given node.

	/**
	 * Neighbor iterator.
	 * This can be used to iterate over all the neighboring nodes' IDs of given node ID.
	 */
	class NeighborIterator: public std::iterator<std::input_iterator_tag,
			node_id_t>
	{
	public:
		/**
		 * Default constructor needed for STL compliance. This creates a singular
		 * (invalid) iterator which does not belong to any network.
		 */
		NeighborIterator();
		/**
		 * Constructor.
		 * @param net MultiNetwork the iterator belongs to.
		 * @param n ID of the node whose neighbors are iterated over.
		 * @param val Current value (index in the nodes link list).
		 */
		NeighborIterator(const MultiNetwork& net, node_id_t n,
				Node::LinkSet::iterator val);
		/**
		 * Copy constructor.
		 * @param it %NeighborIterator to copy.
		 */
		NeighborIterator(const NeighborIterator& it);
		/**
		 * Destructor.
		 */
		~NeighborIterator();

		/**
		 * Assignment operator.
		 * @param it %NeighborIterator to assign from.
		 * @return reference to self
		 */
		NeighborIterator& operator=(const NeighborIterator& it);
		/**
		 * Equality operator.
		 * @param it %NeighborIterator to check equality with.
		 * @return true if equal
		 */
		bool operator==(const NeighborIterator& it);
		/**
		 * Unequality operator.
		 * @param it %NeighborIterator to check unequality with.
		 * @return true if unequal
		 */
		bool operator!=(const NeighborIterator& it);
		/**
		 * Prefix increment operator.
		 * @return reference to (incremented) self
		 */
		NeighborIterator& operator++();
		/**
		 * Postfix increment operator.
		 * @return copy of self before increment
		 */
		NeighborIterator operator++(int);
		/**
		 * Dereference operator. This iterator dereferences to a node's ID.
		 * @return ID of current neighbor.
		 */
		node_id_t operator*();
	private:
		const MultiNetwork* net_; ///< network the iterator belongs to
		node_id_t n_; ///< node ID the iterator is for
		Node::LinkSet::iterator cur_; ///< current iterator position
	}; // class MultiNetwork::NeighborIterator

	typedef std::pair<NeighborIterator, NeighborIterator> NeighborIteratorRange; ///< Neighbor iterator range type.

public:
	/**
	 * Default constructor. Creates an empty network of zero nodes and links.
	 */
	MultiNetwork();

	/**
	 * Constructor.
	 *
	 * Creates an empty network with @p nNodes nodes, each of which can be in any
	 * of the @p nNodeStates states. Memory is allocated to allow for
	 * the storage of @p nLinks links (this will be enlarged as soon as necessary).
	 * @p nLinkStates is the number of possible link states.
	 * The supplied LinkStateCalculator object @p lsCalc will be used to keep
	 * link and node states consistent.
	 * @param nNodes Number of nodes in the network.
	 * @param nLinks Number of links to reserve memory for.
	 * @param nNodeStates Number of possible node states.
	 * @param nLinkStates Number of possible link states.
	 * @param lsCalc LinkStateCalculator object.
	 */
	MultiNetwork(id_size_t nNodes, id_size_t nLinks, id_size_t nNodeStates,
			id_size_t nLinkStates, LinkStateCalculator* lsCalc);
	virtual ~MultiNetwork();

private:
	/* FIXME boost::graph needs copy-constructible graph concepts
	 *
	 MultiNetwork(const MultiNetwork& net); // disallow copying
	 */
	MultiNetwork& operator=(const MultiNetwork& net); // disallow assignment

public:
	/**
	 * Set the LinkStateCalculator object to use in order to keep node and link
	 * states consistent.
	 * @param lsCalc pointer to LinkStateCalculator object
	 */
	void setLinkStateCalculator(LinkStateCalculator* lsCalc);

	/**
	 * Return a copy of the internal link state calculator.
	 * @return LinkStateCalculator object.
	 */
	const LinkStateCalculator& getLinkStateCalculator() const;

	/**
	 * Reset to empty network.
	 * Deletes all nodes and links and re-creates an empty network with @p nNodes nodes,
	 * each of which can be in any of the @p nNodeStates states. Memory is allocated
	 * to allow for the storage of @p nLinks links. @p nLinkStates is the number
	 * of possible link states.
	 *
	 * FIXME reset() will not work this way.
	 * @todo Create factory class for networks. This should be able to create new network objects
	 * from file input for instance.
	 *
	 * @deprecated Should be removed from future versions. Use factory methods instead.
	 *
	 * @param nNodes Number of nodes in the network.
	 * @param nLinks Number of links to reserve memory for.
	 * @param nNodeStates Number of possible node states.
	 * @param nLinkStates Number of possible link states.
	 */
	void reset(id_size_t nNodes, id_size_t nLinks,
			node_state_size_t nNodeStates, link_state_size_t nLinkStates,
			LinkStateCalculator* lsCalc);

	/**
	 * Return the number of nodes in the network.
	 * @return Number of nodes
	 */
	id_size_t numberOfNodes() const;
	/**
	 * Return the number of nodes in state @p s in the network.
	 * @param s State to count nodes in.
	 * @return Number of nodes in state @p s.
	 */
	id_size_t numberOfNodes(node_state_t s) const;
	/**
	 * Return the number of links in the network.
	 * @return Number of links.
	 */
	id_size_t numberOfLinks() const;
	/**
	 * Return the number of links in state @p s in the network.
	 * @param s State to count links in.
	 * @return Number of links in state @p s.
	 */
	id_size_t numberOfLinks(link_state_t s) const;
	/**
	 * Get number of possible node states.
	 * @return Number of possible node states.
	 */
	node_state_size_t numberOfNodeStates() const;
	/**
	 * Get number of possible link states.
	 * @return Number of possible link states.
	 */
	link_state_size_t numberOfLinkStates() const;

	/**
	 * Add a new node to category 0.
	 * @return	Unique ID of the new node.
	 */
	node_id_t addNode();

	/**
	 * Add a new node in state @p s.
	 * @param s State of the new node.
	 * @return	Unique ID of the new node.
	 */
	node_id_t addNode(node_state_t s);

	/**
	 * Create a link between two nodes, given by their unique IDs. The link
	 * state is determined using the internal link state calculator.
	 * @param source Unique ID of the source node
	 * @param target Unique ID of the target node
	 * @return Unique ID of link created
	 */
	virtual link_id_t addLink(node_id_t source, node_id_t target);

	/**
	 * Change link to connect the new @p source with the new @p target.
	 * If you need link rewiring, use this instead of removing and
	 * adding a link. Here, the link is not deleted, thus keeping its link ID.
	 * Returns true.
	 * @param l	Unique ID of link to change.
	 * @param source New source node ID.
	 * @param target New target node ID.
	 * @return true
	 */
	virtual bool changeLink(link_id_t l, node_id_t source, node_id_t target);

	/**
	 * Remove link with unique ID @p l
	 * @param l ID of link to remove.
	 */
	void removeLink(link_id_t l);

	/**
	 * Remove all links in network.
	 */
	void removeAllLinks();

	/**
	 * Remove node with unique ID @p n.
	 * @param n	ID of node to be removed.
	 */
	void removeNode(node_id_t n);

	/**
	 * Check if there exists a direct link between two nodes.
	 * @param source ID of source node.
	 * @param target ID of target node.
	 * @return A pair where the first part is true if the link exists, and the second part
	 * gives its link ID. Note that the second part is meaningful only if the first part is true.
	 */
	std::pair<bool, link_id_t> isLink(node_id_t source, node_id_t target) const;

	/**
	 * Degree of node @p n.
	 * @param n ID of node.
	 * @return %Node degree.
	 */
	id_size_t degree(node_id_t n) const;

	/**
	 *	Put node with ID @p n in state @p s.
	 *	@param n %Node ID of node.
	 *	@param s New state of the node.
	 */
	void setNodeState(node_id_t n, node_state_t s);
	/**
	 * Return state of node with ID @p n.
	 * @param n Node ID.
	 * @return State of the node.
	 */
	node_state_t getNodeState(node_id_t n) const;

public:
	/**
	 * Return state of link with ID @p l.
	 * @param l Link ID.
	 * @return Link state.
	 */
	link_state_t getLinkState(link_id_t l) const;

	/**
	 * Return iterator range of all nodes in the network.
	 * @return @c std::pair of NodeIterators, where the first points to the
	 * first node in the network and the second to the past-the-end node.
	 */
	NodeIteratorRange nodes() const;
	/**
	 * Return iterator range of all nodes in state @p s.
	 * @return @c std::pair of NodeIterators, where the first points to the
	 * first node in state @p s and the second to the past-the-end node
	 * in this state.
	 */
	NodeStateIteratorRange nodes(node_state_t s) const;
	/**
	 * Return iterator range of all links in the network.
	 * @return @c std::pair of LinkIterators, where the first points to the
	 * first link in the network and the second to the past-the-end link.
	 */
	LinkIteratorRange links() const;
	/**
	 * Return iterator range of all links in state @p s.
	 * @return @c std::pair of LinksIterators, where the first points to the
	 * first link in the state @p s and the second to the past-the-end link
	 * in this state.
	 */
	LinkStateIteratorRange links(link_state_t s) const;

	/**
	 * Get random node.
	 * @return Pair of bool and node ID. If @p first is false, the network is empty
	 * and the returned node ID is invalid.
	 */
	std::pair<bool, node_id_t> randomNode() const;
	/**
	 * Get random node in state @p s.
	 * @param s %Node state to choose from.
	 * @return Pair of bool and node ID. If @p first is false, there are no nodes
	 * in state @p s and the returned node ID is invalid.
	 */
	std::pair<bool, node_id_t> randomNode(node_state_t s) const;
	/**
	 * Get random link.
	 * @return Pair of bool and link ID. If @p first is false, the network has no links
	 * and the returned link ID is invalid.
	 */
	std::pair<bool, link_id_t> randomLink() const;
	/**
	 * Get random link in state @p s.
	 * @param s %Link state to choose from.
	 * @return Pair of bool and link ID. If @p first is false, there are no links in state
	 * @p s and the returned link ID is invalid.
	 */
	std::pair<bool, link_id_t> randomLink(link_state_t s) const;

	/**
	 * Get random link adjacent to node @p n.
	 * @param n %Node ID
	 * @return Pair of bool and link ID. If @p first is false, node @p n has no neighbors and
	 * the returned link ID is invalid.
	 */
	std::pair<bool, link_id_t> randomNeighborLink(node_id_t n) const;
	/**
	 * Get random neighbor of node @p n.
	 * @param n %Node ID
	 * @return Pair of bool and node ID. If @p first is false, node @p n has no neighbors and
	 * the returned node ID is invalid.
	 */
	std::pair<bool, node_id_t> randomNeighbor(node_id_t n) const;

	/**
	 * Return iterator range of all neighboring nodes of node @p n.
	 * @param n %Node ID of node whose neighbors should be iterated over.
	 * @return NeighborIteratorRange
	 */
	NeighborIteratorRange neighbors(node_id_t n) const;

	/**
	 * Return iterator range of all neighboring links of node @p n.
	 * @param n %Node ID
	 * @return Iterator range for neighboring link IDs.
	 */
	NeighborLinkIteratorRange neighborLinks(node_id_t n) const;

	/**
	 * Get source node ID of link @p l.
	 * @param l %Link ID
	 * @return %Node ID of link source.
	 */
	node_id_t source(link_id_t l) const;
	/**
	 * Get target node ID of link @p l.
	 * @param l %Link ID
	 * @return %Node ID of link target.
	 */
	node_id_t target(link_id_t l) const;

protected:
	/**
	 * Initialize network with @p nodes Nodes.
	 * @param nodes Number of nodes to create.
	 */
	void init(id_size_t nodes);

	/**
	 * Clear the network, deleting all nodes and edges.
	 */
	void clear();

	/**
	 * Return reference to Node object with unique ID @p n.
	 * @return Reference to Node.
	 */
	Node& node(node_id_t n) const;
	/**
	 * Return reference to Link object with unique ID @p l.
	 * @return Reference to Link.
	 */
	Link& link(link_id_t l) const;
	/**
	 * Recalculate states of links adjacent to node @p n.
	 * @param n %Node ID.
	 */
	void recalcLinkStates(node_id_t n);
	bool isValidLinkStateCalculator(LinkStateCalculator* lc) const;

private:
	NodeRepo* nodeStore_; ///< repository of nodes
	LinkRepo* linkStore_; ///< repository of links
	LinkStateCalculator* lsCalc_; ///< link state calculator
	bool lscOwn_; ///< true if we need to manage the link state calculator
};

inline id_size_t MultiNetwork::numberOfNodes() const
{
	return nodeStore_->size();
}

inline id_size_t MultiNetwork::numberOfNodes(const node_state_t s) const
{
	return nodeStore_->count(s);
}

inline id_size_t MultiNetwork::numberOfLinks() const
{
	return linkStore_->size();
}

inline id_size_t MultiNetwork::numberOfLinks(const link_state_t s) const
{
	return linkStore_->count(s);
}

inline Node& MultiNetwork::node(const node_id_t n) const
{
	Node* pt = (*nodeStore_)[n];
	assert(pt != 0);
	return *pt;
}

inline Link& MultiNetwork::link(const link_id_t l) const
{
	Link* pt = (*linkStore_)[l];
	assert(pt != 0);
	return *pt;
}

inline node_id_t MultiNetwork::source(const link_id_t l) const
{
	return link(l).source();
}

inline node_id_t MultiNetwork::target(const link_id_t l) const
{
	return link(l).target();
}

inline MultiNetwork::NodeIteratorRange MultiNetwork::nodes() const
{
	return nodeStore_->ids();
}

inline MultiNetwork::NodeStateIteratorRange MultiNetwork::nodes(
		const node_state_t s) const
{
	return nodeStore_->ids(s);
}

inline MultiNetwork::LinkIteratorRange MultiNetwork::links() const
{
	return linkStore_->ids();
}

inline MultiNetwork::LinkStateIteratorRange MultiNetwork::links(
		const link_state_t s) const
{
	return linkStore_->ids(s);
}

inline id_size_t MultiNetwork::degree(const node_id_t n) const
{
	return node(n).degree();
}

inline node_state_t MultiNetwork::getNodeState(const node_id_t n) const
{
	return nodeStore_->category(n);
}

inline link_state_t MultiNetwork::getLinkState(const link_id_t l) const
{
	return linkStore_->category(l);
}

inline node_state_size_t MultiNetwork::numberOfNodeStates() const
{
	return nodeStore_->numberOfCategories();
}

inline link_state_size_t MultiNetwork::numberOfLinkStates() const
{
	return linkStore_->numberOfCategories();
}

inline const LinkStateCalculator& MultiNetwork::getLinkStateCalculator() const
{
	return *lsCalc_;
}

}

#endif /*MULTINETWORK_H_*/
