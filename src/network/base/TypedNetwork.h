/**
 * @file TypedNetwork.h
 * @date May 12, 2009
 * @author Gerd Zschaler
 */

#ifndef TYPEDNETWORK_H_
#define TYPEDNETWORK_H_

#include "types.h"
#include "BasicNetwork.h"
#include "state_calculators.h"
#include "../../repo/CategorizedRepository.h"
#include "../../myrng1.2/myrngWELL.h"
#include <iterator>
#include <cassert>
#include <utility> // for std::pair
#include <stdexcept>
#include <string>
#include <sstream>

namespace lnet
{
/**
 * Abstract base class template providing the common network interface. The @p _Node and @p _Link types
 * must implement the Node and Link concepts.
 */
template<class _Node, class _Link>
class TypedNetwork: public BasicNetwork
{
protected:
	// member types
typedef	typename repo::CategorizedRepository<_Node> NodeRepo; ///< CategorizedRepository of Node pointers
	typedef typename repo::CategorizedRepository<_Link> LinkRepo; ///< CategorizedRepository of Link pointers

public:
	// member types
	typedef _Node NodeType;
	typedef _Link LinkType;
	typedef typename NodeRepo::IndexIterator NodeIterator; ///< %Node ID iterator type.
	typedef typename NodeRepo::CategoryIterator NodeStateIterator; ///< %Node ID in state iterator type.
	typedef typename NodeRepo::IndexIteratorRange NodeIteratorRange; ///< %Node ID iterator range type.
	typedef typename NodeRepo::CategoryIteratorRange NodeStateIteratorRange; ///< %Node ID in state iterator range type.
	typedef typename LinkRepo::IndexIterator LinkIterator; ///< %Link ID iterator type.
	typedef typename LinkRepo::CategoryIterator LinkStateIterator; ///< %Link ID in state iterator type.
	typedef typename LinkRepo::IndexIteratorRange LinkIteratorRange; ///< %Link ID iterator range type.
	typedef typename LinkRepo::CategoryIteratorRange LinkStateIteratorRange; ///< %Link ID in state iterator range type.

	typedef typename NodeType::LinkIDIterator NeighborLinkIterator; ///< %Link ID iterator type for neighboring links of a given node.
	typedef typename NodeType::LinkIDIteratorRange NeighborLinkIteratorRange; ///< %Link ID iterator range type for neighboring links of a given node.

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
		NeighborIterator(const TypedNetwork& net, node_id_t n,
				typename NodeType::LinkSet::iterator val);
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
		const TypedNetwork<NodeType, LinkType>* net_; ///< network the iterator belongs to
		node_id_t n_; ///< node ID the iterator is for
		typename NodeType::LinkSet::iterator cur_; ///< current iterator position
	}; // class TypedNetwork::NeighborIterator

	typedef std::pair<NeighborIterator, NeighborIterator> NeighborIteratorRange; ///< Neighbor iterator range type.

public:
	/**
	 * Default constructor. Creates an empty network of zero nodes and links.
	 */
	TypedNetwork();

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
	TypedNetwork(id_size_t nNodes, id_size_t nLinks, node_state_size_t nNodeStates,
			link_state_size_t nLinkStates, LinkStateCalculator* lsCalc);
	virtual ~TypedNetwork();

	std::string info() const;

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
	 * Return state of node with ID @p n.
	 * @param n Node ID.
	 * @return State of the node.
	 */
	node_state_t getNodeState(node_id_t n) const;

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
	virtual void reset(id_size_t nNodes, id_size_t nLinks,
			node_state_size_t nNodeStates, link_state_size_t nLinkStates,
			LinkStateCalculator* lsCalc);
private:
	/* FIXME boost::graph needs copy-constructible graph concepts
	 *
	 MultiNetwork(const MultiNetwork& net); // disallow copying
	 */
	TypedNetwork& operator=(const TypedNetwork& net); // disallow assignment

protected:
	virtual node_id_t doAddNode();
	virtual node_id_t doAddNode(node_state_t s);
	virtual link_id_t doAddLink(node_id_t source, node_id_t target);
	virtual bool
	doChangeLink(link_id_t l, node_id_t source, node_id_t target);
	virtual void doRemoveLink(link_id_t l);
	virtual void doRemoveNode(node_id_t n);
	virtual void doRemoveAllLinks();
	virtual void doSetNodeState(node_id_t n, node_state_t s);

	/**
	 * Initialize network with @p nodes Nodes.
	 * @param nodes Number of nodes to create.
	 */
	virtual void init(id_size_t nodes);

	/**
	 * Return reference to Node object with unique ID @p n.
	 * @return Reference to Node.
	 */
	NodeType& node(node_id_t n) const;
	/**
	 * Return reference to Link object with unique ID @p l.
	 * @return Reference to Link.
	 */
	LinkType& link(link_id_t l) const;

	/**
	 * Called whenever a node changes its state. Thus, link states can be
	 * updated accordingly.
	 * @param n ID of node that changed its state
	 */
	virtual void onNodeStateChange(node_id_t n);

	/**
	 * Recalculate states of links adjacent to node @p n.
	 * @param n %Node ID.
	 */
	void recalcLinkStates(node_id_t n);
	/**
	 * Recalculate states of all links.
	 */
	void recalcLinkStates();
	/**
	 * Check whether @lc refers to a valid LinkStateCalculator object, i.e.
	 * which returns at most the current maximum number of link states.
	 * @param lc Pointer to LinkStateCalculator object
	 * @return true if valid
	 */
	bool isValidLinkStateCalculator(LinkStateCalculator* lc) const;

protected:
	NodeRepo& nodeStore() const;
	LinkRepo& linkStore() const;
	LinkStateCalculator& lsCalc() const;

private:
	NodeRepo* nodeStore_; ///< repository of nodes
	LinkRepo* linkStore_; ///< repository of links
	LinkStateCalculator* lsCalc_; ///< link state calculator
	bool lscOwn_; ///< true if we need to manage the link state calculator
};

template <class _Node, class _Link>
inline std::string TypedNetwork<_Node, _Link>::info() const
{
	std::stringstream ss;
	ss << "Network of N = " << numberOfNodes() << " nodes in " << numberOfNodeStates()
	<< " possible states and L = " << numberOfLinks() << " links in "
	<< numberOfLinkStates() << " possible states.";
	return ss.str();
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::NodeRepo& TypedNetwork<_Node, _Link>::nodeStore() const
{
	return *nodeStore_;
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::LinkRepo& TypedNetwork<_Node, _Link>::linkStore() const
{
	return *linkStore_;
}

template <class _Node, class _Link>
inline LinkStateCalculator& TypedNetwork<_Node, _Link>::lsCalc() const
{
	assert(lsCalc_ != 0);
	return *lsCalc_;
}

template <class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::numberOfNodes() const
{
	return nodeStore_->size();
}

template <class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::numberOfNodes(const node_state_t s) const
{
	return nodeStore_->count(s);
}

template <class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::numberOfLinks() const
{
	return linkStore_->size();
}

template <class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::numberOfLinks(const link_state_t s) const
{
	return linkStore_->count(s);
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::NodeType& TypedNetwork<_Node, _Link>::node(const node_id_t n) const
{
	return (*nodeStore_)[n];
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::LinkType& TypedNetwork<_Node, _Link>::link(const link_id_t l) const
{
	return (*linkStore_)[l];
}

template <class _Node, class _Link>
inline node_id_t TypedNetwork<_Node, _Link>::source(const link_id_t l) const
{
	return link(l).source();
}

template <class _Node, class _Link>
inline node_id_t TypedNetwork<_Node, _Link>::target(const link_id_t l) const
{
	return link(l).target();
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::NodeIteratorRange TypedNetwork<_Node, _Link>::nodes() const
{
	return nodeStore_->ids();
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::NodeStateIteratorRange TypedNetwork<_Node, _Link>::nodes(
		const node_state_t s) const
{
	return nodeStore_->ids(s);
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::LinkIteratorRange TypedNetwork<_Node, _Link>::links() const
{
	return linkStore_->ids();
}

template <class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::LinkStateIteratorRange TypedNetwork<_Node, _Link>::links(
		const link_state_t s) const
{
	return linkStore_->ids(s);
}

template <class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::degree(const node_id_t n) const
{
	return node(n).degree();
}

template <class _Node, class _Link>
inline node_state_t TypedNetwork<_Node, _Link>::getNodeState(const node_id_t n) const
{
	return nodeStore_->category(n);
}

template <class _Node, class _Link>
inline link_state_t TypedNetwork<_Node, _Link>::getLinkState(const link_id_t l) const
{
	return linkStore_->category(l);
}

template <class _Node, class _Link>
inline node_state_size_t TypedNetwork<_Node, _Link>::numberOfNodeStates() const
{
	return nodeStore_->numberOfCategories();
}

template <class _Node, class _Link>
inline link_state_size_t TypedNetwork<_Node, _Link>::numberOfLinkStates() const
{
	return linkStore_->numberOfCategories();
}

template <class _Node, class _Link>
inline const LinkStateCalculator& TypedNetwork<_Node, _Link>::getLinkStateCalculator() const
{
	return lsCalc();
}

template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::TypedNetwork() :
BasicNetwork(), nodeStore_(new NodeRepo(1, 0)), linkStore_(new LinkRepo(1, 0)), lsCalc_(new ConstLinkState<> ), lscOwn_(true)
{
}

template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::TypedNetwork(const id_size_t nNodes, const id_size_t nLinks,
		const node_state_size_t nNodeStates, const link_state_size_t nLinkStates,
		LinkStateCalculator* lsCalc) : BasicNetwork(),
nodeStore_(new NodeRepo(nNodeStates, nNodes)), linkStore_(new LinkRepo(
				nLinkStates, nLinks)), lsCalc_(lsCalc), lscOwn_(false)
{
	init(nNodes);
}

template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::~TypedNetwork()
{
	delete nodeStore_;
	delete linkStore_;
	if (lscOwn_)
	delete lsCalc_;
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::setLinkStateCalculator(LinkStateCalculator* lsCalc)
{
	if (lscOwn_)
	{
		delete lsCalc_;
		lscOwn_ = false;
	}
	if (lsCalc != 0)
	{
		if (isValidLinkStateCalculator(lsCalc))
		lsCalc_ = lsCalc;
		else
		throw(std::invalid_argument(
						"LinkStateCalculator gives impossible link states!"));
	}
	else
	{
		lsCalc_ = new ConstLinkState<>;
		lscOwn_ = true;
	}
	recalcLinkStates();
}

template<class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::isValidLinkStateCalculator(LinkStateCalculator* lc) const
{
	bool retval = true;
	for (node_state_t i = 0; i < numberOfNodeStates(); ++i)
	{
		for (node_state_t j = 0; j < numberOfNodeStates(); ++j)
		{
			retval &= ((*lc)(i, j) < numberOfLinkStates());
		}
	}
	return retval;
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::init(id_size_t nodes)
{
	for (id_size_t i = 0; i < nodes; ++i)
	{
		(*nodeStore_) << NodeType();
	}
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::reset(const id_size_t nNodes, const id_size_t nLinks,
		const node_state_size_t nNodeStates,
		const link_state_size_t nLinkStates, LinkStateCalculator* lsCalc)
{
	delete nodeStore_;
	delete linkStore_;
	nodeStore_ = new NodeRepo(nNodeStates, nNodes);
	linkStore_ = new LinkRepo(nLinkStates, nLinks);
	setLinkStateCalculator(lsCalc);
	init(nNodes);
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doRemoveLink(const link_id_t l)
{
	LinkType& theLink = link(l);
	node(theLink.source()).removeLink(l);
	node(theLink.target()).removeLink(l);
	linkStore_->remove(l);
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doRemoveAllLinks()
{
	NodeIteratorRange iters = nodes();
	for (NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		node(*it).clear();
	}
	linkStore_->removeAll();
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doRemoveNode(const node_id_t n)
{
	typename NodeType::LinkIDIteratorRange iters = node(n).links();
	for (typename NodeType::LinkIDIterator& li = iters.first; li != iters.second; ++li)
	{
		// remove neighbors' references to connecting links
		LinkType& l = link(*li);
		node_id_t m = (l.source() == n) ? l.target() : l.source();
		node(m).removeLink(*li);
		// delete connecting links
		linkStore_->remove(*li);
	}
	nodeStore_->remove(n);
}

template <class _Node, class _Link>
node_id_t TypedNetwork<_Node, _Link>::doAddNode()
{
	return doAddNode(0);
}

template <class _Node, class _Link>
node_id_t TypedNetwork<_Node, _Link>::doAddNode(const node_state_t s)
{
	return nodeStore_->insert(NodeType(), s);
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doSetNodeState(const node_id_t n, const node_state_t s)
{
	nodeStore_->setCategory(n, s);
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::onNodeStateChange(const node_id_t n)
{
	recalcLinkStates(n);
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::recalcLinkStates(const node_id_t n)
{
	NeighborLinkIteratorRange iters = neighborLinks(n);
	for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
	{
		linkStore_->setCategory(*it, (*lsCalc_)(getNodeState(source(*it)),
						getNodeState(target(*it))));
	}
}

template <class _Node, class _Link>
void TypedNetwork<_Node, _Link>::recalcLinkStates()
{
	LinkIteratorRange iters = links();
	for (LinkIterator& it = iters.first; it != iters.second; ++it)
	{
		linkStore_->setCategory(*it, (*lsCalc_)(getNodeState(source(*it)), getNodeState(target(*it))));
	}
}

template <class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::isLink(const node_id_t source,
		const node_id_t target) const
{
	node_id_t a, b;
	if (node(source).degree() < node(target).degree())
	{
		a = source;
		b = target;
	}
	else
	{
		a = target;
		b = source;
	}

	typename NodeType::LinkIDIteratorRange iters = node(a).links();
	if (a == b)
	{
		for (typename NodeType::LinkIDIterator& it = iters.first; it != iters.second; ++it)
		{
			if (link(*it).isLoop())
			{
				return std::make_pair(true, *it);
			}
		}
	}
	else
	{
		for (typename NodeType::LinkIDIterator& it = iters.first; it != iters.second; ++it)
		{
			if (link(*it).connectsTo(b))
			{
				return std::make_pair(true, *it);
			}
		}
	}
	return std::make_pair(false, 0);
}

template <class _Node, class _Link>
std::pair<bool, node_id_t> TypedNetwork<_Node, _Link>::randomNode() const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (nodeStore_->size() > 0)
	{
		ret.first = true;
		ret.second = nodeStore_->id(
				static_cast<repo::address_t> (rng.IntFromTo(0,
								nodeStore_->size() - 1)));
	}
	return ret;
}

template <class _Node, class _Link>
std::pair<bool, node_id_t> TypedNetwork<_Node, _Link>::randomNode(const node_state_t s) const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (nodeStore_->count(s) > 0)
	{
		ret.first = true;
		ret.second = nodeStore_->id(s,
				static_cast<repo::address_t> (rng.IntFromTo(0,
								nodeStore_->count(s) - 1)));
	}
	return ret;
}

template <class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::randomLink() const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (linkStore_->size() > 0)
	{
		ret.first = true;
		ret.second = linkStore_->id(
				static_cast<repo::address_t> (rng.IntFromTo(0,
								linkStore_->size() - 1)));
	}
	return ret;
}

template <class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::randomLink(const link_state_t s) const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (linkStore_->count(s) > 0)
	{
		ret.first = true;
		ret.second = linkStore_->id(s,
				static_cast<repo::address_t> (rng.IntFromTo(0,
								linkStore_->count(s) - 1)));
	}
	return ret;
}

template <class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::randomNeighborLink(const node_id_t n) const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (degree(n) != 0)
	{
		ret.first = true;
		const int num = rng.IntFromTo(0, degree(n));
		int i = 0;
		NeighborLinkIteratorRange iters = neighborLinks(n);
		for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
		{
			if (i == num)
			{
				ret.second = *it;
				break;
			}
			++i;
		}
	}
	return ret;
}

template <class _Node, class _Link>
std::pair<bool, node_id_t> TypedNetwork<_Node, _Link>::randomNeighbor(const node_id_t n) const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (degree(n) != 0)
	{
		ret.first = true;
		const int num = rng.IntFromTo(0, degree(n));
		int i = 0;
		NeighborIteratorRange iters = neighbors(n);
		for (NeighborIterator& it = iters.first; it != iters.second; ++it)
		{
			if (i == num)
			{
				ret.second = *it;
				break;
			}
			++i;
		}
	}
	return ret;
}

template <class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIteratorRange TypedNetwork<_Node, _Link>::neighbors(const node_id_t n) const
{
	return std::make_pair(NeighborIterator(*this, n, node(n).links().first),
			NeighborIterator(*this, n, node(n).links().second));
}

template <class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborLinkIteratorRange TypedNetwork<_Node, _Link>::neighborLinks(
		const node_id_t n) const
{
	return node(n).links();
}

template <class _Node, class _Link>
link_id_t TypedNetwork<_Node, _Link>::doAddLink(const node_id_t source, const node_id_t target)
{
	const link_state_t s = lsCalc()(getNodeState(source),
			getNodeState(target));
	const link_id_t l = linkStore_->insert(LinkType(source, target), s);
	node(source).addLink(l);
	node(target).addLink(l);
	return l;
}

template <class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::doChangeLink(const link_id_t l, const node_id_t source,
		const node_id_t target)
{
	LinkType& theLink = link(l);
	if (source != theLink.source())
	{
		node(theLink.source()).removeLink(l);
		theLink.setSource(source);
		node(source).addLink(l);
	}
	if (target != theLink.target())
	{
		node(theLink.target()).removeLink(l);
		theLink.setTarget(target);
		node(target).addLink(l);
	}
	linkStore_->setCategory(l, lsCalc()(getNodeState(source), getNodeState(
							target)));
	return true;
}

//================ NeighborIterator implementation ======================
template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::NeighborIterator() :
net_(0), n_(0)
{
}

template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::NeighborIterator(const TypedNetwork<_Node, _Link>& net,
		const node_id_t n, const typename NodeType::LinkSet::iterator val) :
net_(&net), n_(n), cur_(val)
{
}

template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::NeighborIterator(const typename TypedNetwork<_Node, _Link>::NeighborIterator& it) :
net_(it.net_), n_(it.n_), cur_(it.cur_)
{
}

template <class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::~NeighborIterator()
{
}

template <class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIterator& TypedNetwork<_Node, _Link>::NeighborIterator::operator=(
		const typename TypedNetwork<_Node, _Link>::NeighborIterator& it)
{
	net_ = it.net_;
	n_ = it.n_;
	cur_ = it.cur_;
	return *this;
}

template <class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::NeighborIterator::operator==(
		const typename TypedNetwork<_Node, _Link>::NeighborIterator& it)
{
	return (net_ == it.net_) && (cur_ == it.cur_) && (n_ == it.n_);
}

template <class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::NeighborIterator::operator!=(
		const typename TypedNetwork<_Node, _Link>::NeighborIterator& it)
{
	return !this->operator==(it);
}

template <class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIterator& TypedNetwork<_Node, _Link>::NeighborIterator::operator++()
{
	if (net_)
	{
		++cur_;
	}
	return *this;
}

template <class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIterator TypedNetwork<_Node, _Link>::NeighborIterator::operator++(int)
{
	NeighborIterator tmp(*this);
	++(*this);
	return (tmp);
}

template <class _Node, class _Link>
node_id_t TypedNetwork<_Node, _Link>::NeighborIterator::operator*()
{
	if (net_)
	{
		const LinkType& theLink = net_->link(*cur_);
		return (theLink.source() == n_) ? theLink.target() : theLink.source();
	}
	else
	{
		return 0; // FIXME is this wise? singular iterator should not be dereferenced anyway.
	}
}

}

#endif /* TYPEDNETWORK_H_ */
