/**
 * @file TripleNetwork.h
 * @date 13.05.2009
 * @author gerd
 */

#ifndef TRIPLENETWORK_H_
#define TRIPLENETWORK_H_

#include "TripleMultiNetwork.h"
#include "base/traits.h"

namespace lnet
{

class TripleNetwork: public lnet::TripleMultiNetwork
{
public:
	typedef disallow_parallel_edge_tag edge_parallel_category;

	TripleNetwork();
	TripleNetwork(const TripleNetwork& net);
	TripleNetwork(id_size_t nNodes, id_size_t nLinks, id_size_t nNodeStates,
			LinkStateCalculator* lsCalc = 0, TripleStateCalculator* tsCalc = 0);
	virtual ~TripleNetwork();
protected:
	/**
	 * Create a link between two nodes, given by their unique IDs. If the link already exists, no
	 * new link will be added, and the ID of the existing link will be returned.
	 * @param source Unique ID of the source node
	 * @param target Unique ID of the target node
	 * @return Unique ID of link created
	 */
	link_id_t doAddLink(node_id_t source, node_id_t target);

	/**
	 * Change link to connect the new @p source with the new @p target.
	 * If you need link rewiring, use this instead of removing and
	 * adding a link. Here, the link is not deleted, thus keeping its link ID.
	 * @note If the link to be created already exists, i.e. the network contains
	 * a link (@p source, @p target), the method does nothing and returns @c false.
	 * @param l	Unique ID of link to change.
	 * @param source New source node ID.
	 * @param target New target node ID.
	 * @return True if link has been changed, false if the target link exists already.
	 */
	bool doChangeLink(link_id_t l, node_id_t source, node_id_t target);

};

}

#endif /* TRIPLENETWORK_H_ */
