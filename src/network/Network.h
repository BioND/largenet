/**
 * @file Network.h
 * @author Gerd Zschaler
 * @date Nov 12, 2008
 * Declaration of the lnet::Network class.
 */
#ifndef NETWORK_H_
#define NETWORK_H_

#include "types.h"
#include "MultiNetwork.h"

namespace lnet
{

/**
 * A class representing a network of Nodes connected by Links. Parallel edges (multiple
 * links connecting the same nodes) are silently ignored. Self-loops are allowed.
 */
class Network: public lnet::MultiNetwork
{
public:
	/**
	 * Default constructor.
	 * Creates an empty network of zero nodes and links.
	 */
	Network();
	/**
	 * Constructor.
	 *
	 * Creates an empty network with @p nNodes nodes, each of which can be in any
	 * of the @p nNodeStates states. Memory is allocated to allow for
	 * the storage of @p nLinks links (this will be enlarged as soon as necessary).
	 * @p nLinkStates is the number of possible link states.
	 * @param nNodes Number of nodes in the network.
	 * @param nLinks Number of links to reserve memory for.
	 * @param nNodeStates Number of possible node states.
	 * @param nLinkStates Number of possible link states.
	 */
	Network(id_size_t nNodes, id_size_t nLinks, id_size_t nNodeStates,
			id_size_t nLinkStates);

	/**
	 * Destructor.
	 */
	virtual ~Network();

	/**
	 * Create a link between two nodes, given by their unique IDs. The link
	 * will be inserted in category @p 0. If the link already exists, no
	 * new link will be added, and the ID of the existing link will be returned.
	 * @param source Unique ID of the source node
	 * @param target Unique ID of the target node
	 * @return Unique ID of link created
	 */
	virtual link_id_t addLink(node_id_t source, node_id_t target);

	/**
	 * Create a link between two nodes, given by their unique IDs, in state @p s.
	 * If the link already exists, no new link will be added but the
	 * state of the existing link will be changed and its ID will be returned.
	 * @param source Unique ID of the source node
	 * @param target Unique ID of the target node
	 * @param s State of the new link.
	 * @return Unique ID of link created
	 */
	virtual link_id_t addLink(node_id_t source, node_id_t target,
			link_state_t s);

	/**
	 * Change link to connect the new @p source with the new @p target, and set link
	 * state to @p s. If you need link rewiring, use this instead of removing and
	 * adding a link. Here, the link is not deleted, thus keeping its link ID.
	 * @note If the link to be created already exists, i.e. the network contains
	 * a link (@p source, @p target), the method does nothing and returns @c false.
	 * @param l	Unique ID of link to change.
	 * @param source New source node ID.
	 * @param target New target node ID.
	 * @param s New link state.
	 * @return True if link has been changed, false if the target link exists already.
	 */
	virtual bool changeLink(link_id_t l, node_id_t source, node_id_t target,
			link_state_t s);
};

}

#endif /* NETWORK_H_ */
