/**
 * @file VoterModel.h
 * @author Gerd Zschaler <gzschaler@googlemail.com>
 */

#ifndef VOTERMODEL_H_
#define VOTERMODEL_H_

#include <largenet.h>

class VoterModel
{
public:
	static const lnet::node_state_size_t node_states = 2;
	static const lnet::link_state_size_t link_states = 4;

	/// Names for node states
	enum NodeState
	{
		UP, DOWN
	};
	/**
	 * Names for link states
	 *
	 * Note that the link states will be computed automatically by the library from the
	 * states of the two nodes a link connects using a lnet::DefaultLinkStateCalculator.
	 * To use enum names for the automatically computed states, they have to be specified
	 * in canonical order in the enum.
	 *
	 * @see largenet/base/state_calculators.h
	 * @see largenet/motifs/LinkMotif.h
	 */
	enum LinkState
	{
		UU, UD, DD
	};

	VoterModel(lnet::Network& net, double p);
	double step();
	bool stopped() const;

private:
	/**
	 * Rewire link with ID @p l attached to node with ID @p source to a random node in the same
	 * node state, so that it remains attached to @p source.
	 */
	void rewire(lnet::link_id_t l, lnet::node_id_t source);

	lnet::Network& net_;
	double p_;
};

inline bool VoterModel::stopped() const
{
	return (net_.numberOfNodes(UP) == 0) || (net_.numberOfNodes(DOWN) == 0)
			|| (net_.numberOfLinks(UD) == 0);
}

#endif /* VOTERMODEL_H_ */
