/**
 * @file VoterModel.h
 * @date 29.07.2009
 * @author gerd
 */

#ifndef VOTERMODEL_H_
#define VOTERMODEL_H_

#include <largenet.h>

class VoterModel
{
public:
	static const lnet::node_state_size_t node_states = 2;
	static const lnet::link_state_size_t link_states = 4;

	enum NodeState
	{
		UP, DOWN
	};
	enum LinkState
	{
		UU, UD, DD
	};

	VoterModel(lnet::Network& net, double p);
	double step();
	bool stopped() const;

private:
	void rewire(lnet::link_id_t l, lnet::node_id_t source);

	Network& net_;
	double p_;
};

inline bool VoterModel::stopped() const
{
	return (net_.numberOfNodes(UP) == 0) || (net_.numberOfNodes(DOWN) == 0)
			|| (net_.numberOfLinks(UD) == 0);
}

#endif /* VOTERMODEL_H_ */
