/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file TypedNetwork.h
 * @date May 12, 2009
 * @author Gerd Zschaler
 */

#ifndef TYPEDNETWORK_H_
#define TYPEDNETWORK_H_

#include "BasicNetwork.h"
#include "types.h"
#include "traits.h"
#include "../../repo/CategorizedRepository.h"
#include "../../myrng1.2/myrngWELL.h"
#include <iterator>
#include <cassert>
#include <utility> // for std::pair
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
	typedef typename repo::CategorizedRepository<_Node> NodeRepo; ///< CategorizedRepository of Node pointers
	typedef typename repo::CategorizedRepository<_Link> LinkRepo; ///< CategorizedRepository of Link pointers

public:
	// member types
	typedef _Node NodeType;
	typedef _Link LinkType;

	typedef typename NodeType::LinkIDIterator NeighborLinkIterator; ///< %Link ID iterator type for neighboring links of a given node.
	typedef typename NodeType::LinkIDIteratorRange NeighborLinkIteratorRange; ///< %Link ID iterator range type for neighboring links of a given node.

	typedef allow_parallel_edge_tag edge_parallel_category;
	typedef no_count_triples_tag triple_counting_category;

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
	TypedNetwork(id_size_t nNodes, id_size_t nLinks,
			node_state_size_t nNodeStates, LinkStateCalculator* lsCalc = 0);

	/**
	 * Copy constructor.
	 * @param net
	 */
	TypedNetwork(const TypedNetwork& net);

	virtual ~TypedNetwork();

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

private:
	/* FIXME boost::graph needs copy-constructible graph concepts
	 *
	 MultiNetwork(const MultiNetwork& net); // disallow copying
	 */
	TypedNetwork& operator=(const TypedNetwork& net); // disallow assignment

protected:
	node_id_t doAddNode();
	node_id_t doAddNode(node_state_t s);
	link_id_t doAddLink(node_id_t source, node_id_t target);
	bool
	doChangeLink(link_id_t l, node_id_t source, node_id_t target);
	void doRemoveLink(link_id_t l);
	void doRemoveNode(node_id_t n);
	void doRemoveAllLinks();
	void doClear();
	void doSetNodeState(node_id_t n, node_state_t s);

	/**
	 * Initialize network with @p nodes Nodes.
	 * @param nodes Number of nodes to create.
	 */
	void init(id_size_t nodes);

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

	void doReset(id_size_t nNodes, id_size_t nLinks,
			node_state_size_t nNodeStates);

	NodeRepo& nodeStore() const;
	LinkRepo& linkStore() const;

private:
	std::string getInfo() const;

	/**
	 * Return the number of nodes in the network.
	 * @return Number of nodes
	 */
	id_size_t getNumberOfNodes() const;
	/**
	 * Return the number of nodes in state @p s in the network.
	 * @param s State to count nodes in.
	 * @return Number of nodes in state @p s.
	 */
	id_size_t getNumberOfNodes(node_state_t s) const;
	/**
	 * Return the number of links in the network.
	 * @return Number of links.
	 */
	id_size_t getNumberOfLinks() const;
	/**
	 * Return the number of links in state @p s in the network.
	 * @param s State to count links in.
	 * @return Number of links in state @p s.
	 */
	id_size_t getNumberOfLinks(link_state_t s) const;
	/**
	 * Get number of possible node states.
	 * @return Number of possible node states.
	 */
	node_state_size_t getNumberOfNodeStates() const;
	/**
	 * Get number of possible link states.
	 * @return Number of possible link states.
	 */
	link_state_size_t getNumberOfLinkStates() const;

	/**
	 * Check if there exists a direct link between two nodes.
	 * @param source ID of source node.
	 * @param target ID of target node.
	 * @return A pair where the first part is true if the link exists, and the second part
	 * gives its link ID. Note that the second part is meaningful only if the first part is true.
	 */
	std::pair<bool, link_id_t>
	doIsLink(node_id_t source, node_id_t target) const;

	/**
	 * Degree of node @p n.
	 * @param n ID of node.
	 * @return %Node degree.
	 */
	id_size_t getDegree(node_id_t n) const;

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
	 * Get source node ID of link @p l.
	 * @param l %Link ID
	 * @return %Node ID of link source.
	 */
	node_id_t getSource(link_id_t l) const;
	/**
	 * Get target node ID of link @p l.
	 * @param l %Link ID
	 * @return %Node ID of link target.
	 */
	node_id_t getTarget(link_id_t l) const;

	/**
	 * Get random node.
	 * @return Pair of bool and node ID. If @p first is false, the network is empty
	 * and the returned node ID is invalid.
	 */
	std::pair<bool, node_id_t> getRandomNode() const;
	/**
	 * Get random node in state @p s.
	 * @param s %Node state to choose from.
	 * @return Pair of bool and node ID. If @p first is false, there are no nodes
	 * in state @p s and the returned node ID is invalid.
	 */
	std::pair<bool, node_id_t> getRandomNode(node_state_t s) const;
	/**
	 * Get random link.
	 * @return Pair of bool and link ID. If @p first is false, the network has no links
	 * and the returned link ID is invalid.
	 */
	std::pair<bool, link_id_t> getRandomLink() const;
	/**
	 * Get random link in state @p s.
	 * @param s %Link state to choose from.
	 * @return Pair of bool and link ID. If @p first is false, there are no links in state
	 * @p s and the returned link ID is invalid.
	 */
	std::pair<bool, link_id_t> getRandomLink(link_state_t s) const;

	/**
	 * Get random link adjacent to node @p n.
	 * @param n %Node ID
	 * @return Pair of bool and link ID. If @p first is false, node @p n has no neighbors and
	 * the returned link ID is invalid.
	 */
	std::pair<bool, link_id_t> getRandomNeighborLink(node_id_t n) const;
	/**
	 * Get random neighbor of node @p n.
	 * @param n %Node ID
	 * @return Pair of bool and node ID. If @p first is false, node @p n has no neighbors and
	 * the returned node ID is invalid.
	 */
	std::pair<bool, node_id_t> getRandomNeighbor(node_id_t n) const;

	LinkIteratorRange getLinks() const;
	LinkStateIteratorRange getLinks(link_state_t s) const;
	NodeIteratorRange getNodes() const;
	NodeStateIteratorRange getNodes(link_state_t s) const;

private:
	NodeRepo* nodeStore_; ///< repository of nodes
	LinkRepo* linkStore_; ///< repository of links
};

template<class _Node, class _Link>
inline std::string TypedNetwork<_Node, _Link>::getInfo() const
{
	std::stringstream ss;
	ss << "Network of N = " << numberOfNodes() << " nodes in "
			<< numberOfNodeStates() << " possible states and L = "
			<< numberOfLinks() << " links in " << numberOfLinkStates()
			<< " possible states.\nAverage degree: " << 2.0
			* static_cast<double> (numberOfLinks()) / numberOfNodes() << ".";
	return ss.str();
}

template<class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::NodeRepo& TypedNetwork<_Node, _Link>::nodeStore() const
{
	return *nodeStore_;
}

template<class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::LinkRepo& TypedNetwork<_Node, _Link>::linkStore() const
{
	return *linkStore_;
}

template<class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::getNumberOfNodes() const
{
	return nodeStore_->size();
}

template<class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::getNumberOfNodes(
		const node_state_t s) const
{
	return nodeStore_->count(s);
}

template<class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::getNumberOfLinks() const
{
	return linkStore_->size();
}

template<class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::getNumberOfLinks(
		const link_state_t s) const
{
	return linkStore_->count(s);
}

template<class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::NodeType& TypedNetwork<_Node, _Link>::node(
		const node_id_t n) const
{
	return (*nodeStore_)[n];
}

template<class _Node, class _Link>
inline typename TypedNetwork<_Node, _Link>::LinkType& TypedNetwork<_Node, _Link>::link(
		const link_id_t l) const
{
	return (*linkStore_)[l];
}

template<class _Node, class _Link>
inline node_id_t TypedNetwork<_Node, _Link>::getSource(const link_id_t l) const
{
	return link(l).source();
}

template<class _Node, class _Link>
inline node_id_t TypedNetwork<_Node, _Link>::getTarget(const link_id_t l) const
{
	return link(l).target();
}

template<class _Node, class _Link>
inline BasicNetwork::NodeIteratorRange TypedNetwork<_Node, _Link>::getNodes() const
{
	return nodeStore_->ids();
}

template<class _Node, class _Link>
inline BasicNetwork::NodeStateIteratorRange TypedNetwork<_Node, _Link>::getNodes(
		const node_state_t s) const
{
	return nodeStore_->ids(s);
}

template<class _Node, class _Link>
inline BasicNetwork::LinkIteratorRange TypedNetwork<_Node, _Link>::getLinks() const
{
	return linkStore_->ids();
}

template<class _Node, class _Link>
inline BasicNetwork::LinkStateIteratorRange TypedNetwork<_Node, _Link>::getLinks(
		const link_state_t s) const
{
	return linkStore_->ids(s);
}

template<class _Node, class _Link>
inline id_size_t TypedNetwork<_Node, _Link>::getDegree(const node_id_t n) const
{
	return node(n).degree();
}

template<class _Node, class _Link>
inline node_state_t TypedNetwork<_Node, _Link>::getNodeState(const node_id_t n) const
{
	return nodeStore_->category(n);
}

template<class _Node, class _Link>
inline link_state_t TypedNetwork<_Node, _Link>::getLinkState(const link_id_t l) const
{
	return linkStore_->category(l);
}

template<class _Node, class _Link>
inline node_state_size_t TypedNetwork<_Node, _Link>::getNumberOfNodeStates() const
{
	return nodeStore_->numberOfCategories();
}

template<class _Node, class _Link>
inline link_state_size_t TypedNetwork<_Node, _Link>::getNumberOfLinkStates() const
{
	return linkStore_->numberOfCategories();
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::TypedNetwork() :
	BasicNetwork(), nodeStore_(new NodeRepo(1, 0)), linkStore_(new LinkRepo(1,
			0))
{
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::TypedNetwork(const TypedNetwork<_Node, _Link>& net) :
	BasicNetwork(net), nodeStore_(new NodeRepo(*net.nodeStore_)), linkStore_(
			new LinkRepo(*net.linkStore_))
{
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::TypedNetwork(const id_size_t nNodes,
		const id_size_t nLinks, const node_state_size_t nNodeStates,
		LinkStateCalculator* lsCalc) :
	BasicNetwork(lsCalc), nodeStore_(new NodeRepo(nNodeStates, nNodes)),
			linkStore_(0)
{
	// FIXME this is awkward, but fixes memory leak
	if (lsCalc == 0)
	{
		DefaultLinkStateCalculator dlsc(nNodeStates);
		linkStore_ = new LinkRepo(dlsc.numberOfLinkStates(), nLinks);
		setLinkStateCalculator(0);
	}
	else
	{
		linkStore_ = new LinkRepo(linkStateCalculator().numberOfLinkStates(),
				nLinks);
	}
	init(nNodes);
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::~TypedNetwork()
{
	delete nodeStore_;
	delete linkStore_;
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doReset(const id_size_t nNodes,
		const id_size_t nLinks, const node_state_size_t nNodeStates)
{
	delete nodeStore_;
	delete linkStore_;
	nodeStore_ = new NodeRepo(nNodeStates, nNodes);
	linkStore_ = new LinkRepo(linkStateCalculator().numberOfLinkStates(),
			nLinks);
	init(nNodes);
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
void TypedNetwork<_Node, _Link>::doRemoveLink(const link_id_t l)
{
	LinkType& theLink = link(l);
	node(theLink.source()).removeLink(l);
	node(theLink.target()).removeLink(l);
	linkStore_->remove(l);
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doRemoveAllLinks()
{
	NodeIteratorRange iters = nodes();
	for (NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		node(*it).clear();
	}
	linkStore_->removeAll();
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doRemoveNode(const node_id_t n)
{
	typename NodeType::LinkIDIteratorRange iters = node(n).links();
	for (typename NodeType::LinkIDIterator& li = iters.first; li
			!= iters.second; ++li)
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

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doClear()
{
	removeAllLinks();
	nodeStore_->removeAll();
}

template<class _Node, class _Link>
node_id_t TypedNetwork<_Node, _Link>::doAddNode()
{
	return doAddNode(0);
}

template<class _Node, class _Link>
node_id_t TypedNetwork<_Node, _Link>::doAddNode(const node_state_t s)
{
	return nodeStore_->insert(NodeType(), s);
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::doSetNodeState(const node_id_t n,
		const node_state_t s)
{
	nodeStore_->setCategory(n, s);
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::onNodeStateChange(const node_id_t n)
{
	recalcLinkStates(n);
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::recalcLinkStates(const node_id_t n)
{
	NeighborLinkIteratorRange iters = neighborLinks(n);
	for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
	{
		linkStore_->setCategory(*it, linkStateCalculator()(getNodeState(source(
				*it)), getNodeState(target(*it))));
	}
}

template<class _Node, class _Link>
void TypedNetwork<_Node, _Link>::recalcLinkStates()
{
	LinkIteratorRange iters = links();
	for (LinkIterator& it = iters.first; it != iters.second; ++it)
	{
		linkStore_->setCategory(*it, linkStateCalculator()(getNodeState(source(
				*it)), getNodeState(target(*it))));
	}
}

template<class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::doIsLink(
		const node_id_t source, const node_id_t target) const
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
		for (typename NodeType::LinkIDIterator& it = iters.first; it
				!= iters.second; ++it)
		{
			if (link(*it).isLoop())
			{
				return std::make_pair(true, *it);
			}
		}
	}
	else
	{
		for (typename NodeType::LinkIDIterator& it = iters.first; it
				!= iters.second; ++it)
		{
			if (link(*it).connectsTo(b))
			{
				return std::make_pair(true, *it);
			}
		}
	}
	return std::make_pair(false, 0);
}

template<class _Node, class _Link>
std::pair<bool, node_id_t> TypedNetwork<_Node, _Link>::getRandomNode() const
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

template<class _Node, class _Link>
std::pair<bool, node_id_t> TypedNetwork<_Node, _Link>::getRandomNode(
		const node_state_t s) const
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

template<class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::getRandomLink() const
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

template<class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::getRandomLink(
		const link_state_t s) const
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

template<class _Node, class _Link>
std::pair<bool, link_id_t> TypedNetwork<_Node, _Link>::getRandomNeighborLink(
		const node_id_t n) const
{
	std::pair<bool, link_id_t> ret = std::make_pair(false, 0);
	if (degree(n) != 0)
	{
		const int num = rng.IntFromTo(0, degree(n) - 1);
		NeighborLinkIteratorRange iters = neighborLinks(n);
		NeighborLinkIterator it = iters.first;
		std::advance(it, num);
		ret.first = it != iters.second; // should always give true
		ret.second = *it;
	}
	return ret;
}

template<class _Node, class _Link>
std::pair<bool, node_id_t> TypedNetwork<_Node, _Link>::getRandomNeighbor(
		const node_id_t n) const
{
	std::pair<bool, node_id_t> ret = std::make_pair(false, 0);
	if (degree(n) != 0)
	{
		const int num = rng.IntFromTo(0, degree(n) - 1);
		NeighborIteratorRange iters = neighbors(n);
		NeighborIterator it = iters.first;
		std::advance(it, num);
		ret.first = it != iters.second; // should always give true
		ret.second = *it;
	}
	return ret;
}

template<class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIteratorRange TypedNetwork<_Node,
		_Link>::neighbors(const node_id_t n) const
{
	return std::make_pair(NeighborIterator(*this, n, node(n).links().first),
			NeighborIterator(*this, n, node(n).links().second));
}

template<class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborLinkIteratorRange TypedNetwork<
		_Node, _Link>::neighborLinks(const node_id_t n) const
{
	return node(n).links();
}

template<class _Node, class _Link>
link_id_t TypedNetwork<_Node, _Link>::doAddLink(const node_id_t source,
		const node_id_t target)
{
	const link_state_t s = linkStateCalculator()(getNodeState(source),
			getNodeState(target));
	const link_id_t l = linkStore_->insert(LinkType(source, target), s);
	node(source).addLink(l);
	node(target).addLink(l);
	return l;
}

template<class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::doChangeLink(const link_id_t l,
		const node_id_t source, const node_id_t target)
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
	linkStore_->setCategory(l, linkStateCalculator()(getNodeState(source),
			getNodeState(target)));
	return true;
}

//================ NeighborIterator implementation ======================
template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::NeighborIterator() :
	net_(0), n_(0)
{
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::NeighborIterator(
		const TypedNetwork<_Node, _Link>& net, const node_id_t n,
		const typename NodeType::LinkSet::iterator val) :
	net_(&net), n_(n), cur_(val)
{
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::NeighborIterator(
		const typename TypedNetwork<_Node, _Link>::NeighborIterator& it) :
	net_(it.net_), n_(it.n_), cur_(it.cur_)
{
}

template<class _Node, class _Link>
TypedNetwork<_Node, _Link>::NeighborIterator::~NeighborIterator()
{
}

template<class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIterator& TypedNetwork<_Node,
		_Link>::NeighborIterator::operator=(const typename TypedNetwork<_Node,
		_Link>::NeighborIterator& it)
{
	net_ = it.net_;
	n_ = it.n_;
	cur_ = it.cur_;
	return *this;
}

template<class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::NeighborIterator::operator==(
		const typename TypedNetwork<_Node, _Link>::NeighborIterator& it)
{
	return (net_ == it.net_) && (cur_ == it.cur_) && (n_ == it.n_);
}

template<class _Node, class _Link>
bool TypedNetwork<_Node, _Link>::NeighborIterator::operator!=(
		const typename TypedNetwork<_Node, _Link>::NeighborIterator& it)
{
	return !this->operator==(it);
}

template<class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIterator& TypedNetwork<_Node,
		_Link>::NeighborIterator::operator++()
{
	assert(net_ != 0);
		++cur_;
	return *this;
}

template<class _Node, class _Link>
typename TypedNetwork<_Node, _Link>::NeighborIterator TypedNetwork<_Node, _Link>::NeighborIterator::operator++(
		int)
{
	NeighborIterator tmp(*this);
	++(*this);
	return (tmp);
}

template<class _Node, class _Link>
node_id_t TypedNetwork<_Node, _Link>::NeighborIterator::operator*()
{
	assert(net_ != 0);
	return net_->link(*cur_).opposite(n_);
}

}

#endif /* TYPEDNETWORK_H_ */
