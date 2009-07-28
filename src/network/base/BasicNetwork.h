/**
 * @file BasicNetwork.h
 * @date 27.05.2009
 * @author gerd
 */

#ifndef BASICNETWORK_H_
#define BASICNETWORK_H_

#include "types.h"
#include "../../repo/iterators.h"
#include <utility>
#include <string>

namespace lnet
{

class LinkStateCalculator;

/**
 * Basic network interface class (abstract).
 */
class BasicNetwork
{
public:

	typedef repo::IndexIterator NodeIterator; ///< %Node ID iterator type.
	typedef repo::CategoryIterator NodeStateIterator; ///< %Node ID in state iterator type.
	typedef repo::IndexIteratorRange NodeIteratorRange; ///< %Node ID iterator range type.
	typedef repo::CategoryIteratorRange NodeStateIteratorRange; ///< %Node ID in state iterator range type.
	typedef repo::IndexIterator LinkIterator; ///< %Link ID iterator type.
	typedef repo::CategoryIterator LinkStateIterator; ///< %Link ID in state iterator type.
	typedef repo::IndexIteratorRange LinkIteratorRange; ///< %Link ID iterator range type.
	typedef repo::CategoryIteratorRange LinkStateIteratorRange; ///< %Link ID in state iterator range type.

	BasicNetwork();
	BasicNetwork(LinkStateCalculator* lsCalc);
	BasicNetwork(const BasicNetwork& net);
	virtual ~BasicNetwork();

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
	 * Set the LinkStateCalculator object to use in order to keep node and link
	 * states consistent.
	 * @param lsCalc pointer to LinkStateCalculator object
	 */
	void setLinkStateCalculator(LinkStateCalculator* lsCalc);

	/**
	 * Return a copy of the internal link state calculator.
	 * @return LinkStateCalculator object.
	 */
	const LinkStateCalculator& linkStateCalculator() const;

	void reset(id_size_t nNodes, id_size_t nLinks,
			node_state_size_t nNodeStates, LinkStateCalculator* lsCalc = 0);
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
	link_id_t addLink(node_id_t source, node_id_t target);

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
	bool changeLink(link_id_t l, node_id_t source, node_id_t target);

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
	 * Remove all nodes and links.
	 */
	void clear();

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
	node_state_t nodeState(node_id_t n) const;

	/**
	 * Return state of link with ID @p l.
	 * @param l Link ID.
	 * @return Link state.
	 */
	link_state_t linkState(link_id_t l) const;

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

	//	/**
	//	 * Return iterator range of all neighboring nodes of node @p n.
	//	 * @param n %Node ID of node whose neighbors should be iterated over.
	//	 * @return NeighborIteratorRange
	//	 */
	//	NeighborIteratorRange neighbors(node_id_t n) const;
	//
	//	/**
	//	 * Return iterator range of all neighboring links of node @p n.
	//	 * @param n %Node ID
	//	 * @return Iterator range for neighboring link IDs.
	//	 */
	//	NeighborLinkIteratorRange neighborLinks(node_id_t n) const;

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
	virtual node_id_t doAddNode() = 0;
	virtual node_id_t doAddNode(node_state_t s) = 0;
	virtual link_id_t doAddLink(node_id_t source, node_id_t target) = 0;
	virtual bool
			doChangeLink(link_id_t l, node_id_t source, node_id_t target) = 0;
	virtual void doRemoveLink(link_id_t l) = 0;
	virtual void doRemoveNode(node_id_t n) = 0;
	virtual void doRemoveAllLinks() = 0;
	virtual void doClear() = 0;
	virtual void doSetNodeState(node_id_t n, node_state_t s) = 0;

	virtual void onNodeStateChange(node_id_t n) = 0;

	virtual void recalcLinkStates() = 0;

	/**
	 * Check whether @lc refers to a valid LinkStateCalculator object, i.e.
	 * which returns at most the current maximum number of link states.
	 * @param lc Pointer to LinkStateCalculator object
	 * @return true if valid
	 */
	bool isValidLinkStateCalculator(LinkStateCalculator* lc) const;

private:
	virtual id_size_t getDegree(node_id_t n) const = 0;
	virtual std::string getInfo() const = 0;
	virtual id_size_t getNumberOfNodes() const = 0;
	virtual id_size_t getNumberOfNodes(node_state_t s) const = 0;
	virtual id_size_t getNumberOfLinks() const = 0;
	virtual id_size_t getNumberOfLinks(node_state_t s) const = 0;
	virtual node_state_size_t getNumberOfNodeStates() const = 0;
	virtual link_state_size_t getNumberOfLinkStates() const = 0;

	virtual void doReset(id_size_t nNodes, id_size_t nLinks,
			node_state_size_t nNodeStates) = 0;

	virtual node_state_t getNodeState(node_id_t n) const = 0;
	virtual link_state_t getLinkState(link_id_t l) const = 0;

	virtual node_id_t getSource(link_id_t l) const = 0;
	virtual node_id_t getTarget(link_id_t l) const = 0;

	virtual std::pair<bool, link_id_t> doIsLink(node_id_t source,
			node_id_t target) const = 0;

	virtual std::pair<bool, node_id_t> getRandomNode() const = 0;
	virtual std::pair<bool, node_id_t> getRandomNode(node_state_t s) const = 0;
	virtual std::pair<bool, link_id_t> getRandomLink() const = 0;
	virtual std::pair<bool, link_id_t> getRandomLink(link_state_t s) const = 0;
	virtual std::pair<bool, link_id_t>
			getRandomNeighborLink(node_id_t n) const = 0;
	virtual std::pair<bool, node_id_t> getRandomNeighbor(node_id_t n) const = 0;

	virtual LinkIteratorRange getLinks() const = 0;
	virtual LinkStateIteratorRange getLinks(link_state_t s) const = 0;
	virtual NodeIteratorRange getNodes() const = 0;
	virtual NodeStateIteratorRange getNodes(link_state_t s) const = 0;

private:
	LinkStateCalculator* lsCalc_; ///< link state calculator @todo Use shared_ptr?
	bool lscOwn_; ///< true if we need to manage the link state calculator
};

inline node_id_t BasicNetwork::addNode()
{
	return doAddNode();
}

inline node_id_t BasicNetwork::addNode(const node_state_t s)
{
	return doAddNode(s);
}

inline link_id_t BasicNetwork::addLink(const node_id_t source,
		const node_id_t target)
{
	return doAddLink(source, target);
}

inline bool BasicNetwork::changeLink(const link_id_t l, const node_id_t source,
		const node_id_t target)
{
	return doChangeLink(l, source, target);
}

inline id_size_t BasicNetwork::degree(const node_id_t n) const
{
	return getDegree(n);
}

inline std::string BasicNetwork::info() const
{
	return getInfo();
}

inline const LinkStateCalculator& BasicNetwork::linkStateCalculator() const
{
	assert(lsCalc_ != 0);
	return *lsCalc_;
}

inline std::pair<bool, link_id_t> BasicNetwork::isLink(const node_id_t source,
		const node_id_t target) const
{
	return doIsLink(source, target);
}

inline link_state_t BasicNetwork::linkState(const link_id_t l) const
{
	return getLinkState(l);
}

inline node_state_t BasicNetwork::nodeState(const node_id_t n) const
{
	return getNodeState(n);
}

inline id_size_t BasicNetwork::numberOfLinks() const
{
	return getNumberOfLinks();
}

inline id_size_t BasicNetwork::numberOfLinks(const link_state_t s) const
{
	return getNumberOfLinks(s);
}

inline id_size_t BasicNetwork::numberOfNodes() const
{
	return getNumberOfNodes();
}

inline id_size_t BasicNetwork::numberOfNodes(const node_state_t s) const
{
	return getNumberOfNodes(s);
}

inline node_state_size_t BasicNetwork::numberOfNodeStates() const
{
	return getNumberOfNodeStates();
}

inline link_state_size_t BasicNetwork::numberOfLinkStates() const
{
	return getNumberOfLinkStates();
}

inline std::pair<bool, link_id_t> BasicNetwork::randomLink() const
{
	return getRandomLink();
}

inline std::pair<bool, link_id_t> BasicNetwork::randomLink(const link_state_t s) const
{
	return getRandomLink(s);
}

inline std::pair<bool, link_id_t> BasicNetwork::randomNeighborLink(
		const node_id_t n) const
{
	return getRandomNeighborLink(n);
}

inline std::pair<bool, node_id_t> BasicNetwork::randomNeighbor(
		const node_id_t n) const
{
	return getRandomNeighbor(n);
}

inline std::pair<bool, node_id_t> BasicNetwork::randomNode() const
{
	return getRandomNode();
}

inline std::pair<bool, node_id_t> BasicNetwork::randomNode(const node_state_t s) const
{
	return getRandomNode(s);
}

inline void BasicNetwork::removeLink(const link_id_t l)
{
	doRemoveLink(l);
}

inline void BasicNetwork::removeNode(const node_id_t n)
{
	doRemoveNode(n);
}

inline void BasicNetwork::removeAllLinks()
{
	doRemoveAllLinks();
}

inline void BasicNetwork::clear()
{
	doClear();
}

inline void BasicNetwork::setNodeState(const node_id_t n, const node_state_t s)
{
	doSetNodeState(n, s);
	onNodeStateChange(n);
}

inline node_id_t BasicNetwork::source(const link_id_t l) const
{
	return getSource(l);
}

inline node_id_t BasicNetwork::target(const link_id_t l) const
{
	return getTarget(l);
}

inline BasicNetwork::NodeIteratorRange BasicNetwork::nodes() const
{
	return getNodes();
}

inline BasicNetwork::NodeStateIteratorRange BasicNetwork::nodes(
		const node_state_t s) const
{
	return getNodes(s);
}

inline BasicNetwork::LinkStateIteratorRange BasicNetwork::links(
		const link_state_t s) const
{
	return getLinks(s);
}

inline BasicNetwork::LinkIteratorRange BasicNetwork::links() const
{
	return getLinks();
}
}

#endif /* BASICNETWORK_H_ */
