#ifndef SWARMMODEL_H_
#define SWARMMODEL_H_

#include "StateSwitchEvent.h"
#include "StateAlignEvent.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>
#include <vector>

typedef std::vector<StateSwitchEvent> event_list;
typedef std::vector<StateAlignEvent> align_event_list;

class SwarmModel
{
public:
	static const lnet::node_state_size_t node_states = 6;
	static const lnet::link_state_size_t link_states = 21;
	static const lnet::link_state_size_t triple_states = 126;

	typedef lnet::TripleNetwork NetType;

	/* Node states
	 * The capital in front shows the temporal direction
	 * The following states whether the node is uninf(uninformed), linf(left informed) or rinf(right informed)
	 */
	enum NodeState
	{
		Luninf, Llinf, Lrinf, Runinf, Rlinf, Rrinf
	};

	/* Link states
	 * All possible combinations for two neighboring node states
	 * Note that AB = BA in terms of link states
	 * The canonical order is used
	 */
	enum LinkState
	{
		LuninfLuninf, LuninfLlinf, LuninfLrinf, LuninfRuninf, LuninfRlinf, LuninfRrinf,
		LlinfLlinf, LlinfLrinf, LlinfRuninf, LlinfRlinf, LlinfRrinf,
		LrinfLrinf, LrinfRuninf, LrinfRlinf, LrinfRrinf,
		RuninfRuninf, RuninfRlinf, RuninfRrinf,
		RlinfRlinf, RlinfRrinf,
		RrinfRrinf
	};

	/* Triplet states
	 */
	enum TripleState
	{
		LuninfLuninfLuninf, LuninfLuninfLlinf, LuninfLuninfLrinf, LuninfLuninfRuninf, LuninfLuninfRlinf, LuninfLuninfRrinf,
		LuninfLlinfLuninf, LuninfLlinfLlinf, LuninfLlinfLrinf, LuninfLlinfRuninf, LuninfLlinfRlinf, LuninfLlinfRrinf,
		LuninfLrinfLuninf, LuninfLrinfLlinf, LuninfLrinfLrinf, LuninfLrinfRuninf, LuninfLrinfRlinf, LuninfLrinfRrinf,
		LuninfRuninfLuninf, LuninfRuninfLlinf, LuninfRuninfLrinf, LuninfRuninfRuninf, LuninfRuninfRlinf, LuninfRuninfRrinf,
		LuninfRlinfLuninf, LuninfRlinfLlinf, LuninfRlinfLrinf, LuninfRlinfRuninf, LuninfRlinfRlinf, LuninfRlinfRrinf,
		LuninfRrinfLuninf, LuninfRrinfLlinf, LuninfRrinfLrinf, LuninfRrinfRuninf, LuninfRrinfRlinf, LuninfRrinfRrinf,
		LlinfLuninfLlinf, LlinfLuninfLrinf, LlinfLuninfRuninf, LlinfLuninfRlinf, LlinfLuninfRrinf,
		LlinfLlinfLlinf, LlinfLlinfLrinf, LlinfLlinfRuninf, LlinfLlinfRlinf, LlinfLlinfRrinf,
		LlinfLrinfLlinf, LlinfLrinfLrinf, LlinfLrinfRuninf, LlinfLrinfRlinf, LlinfLrinfRrinf,
		LlinfRuninfLlinf, LlinfRuninfLrinf, LlinfRuninfRuninf, LlinfRuninfRlinf, LlinfRuninfRrinf,
		LlinfRlinfLlinf, LlinfRlinfLrinf, LlinfRlinfRuninf, LlinfRlinfRlinf, LlinfRlinfRrinf,
		LlinfRrinfLlinf, LlinfRrinfLrinf, LlinfRrinfRuninf, LlinfRrinfRlinf, LlinfRrinfRrinf,
		LrinfLuninfLrinf, LrinfLuninfRuninf, LrinfLuninfRlinf, LrinfLuninfRrinf,
		LrinfLlinfLrinf, LrinfLlinfRuninf, LrinfLlinfRlinf, LrinfLlinfRrinf,
		LrinfLrinfLrinf, LrinfLrinfRuninf, LrinfLrinfRlinf, LrinfLrinfRrinf,
		LrinfRuninfLrinf, LrinfRuninfRuninf, LrinfRuninfRlinf, LrinfRuninfRrinf,
		LrinfRlinfLrinf, LrinfRlinfRuninf, LrinfRlinfRlinf, LrinfRlinfRrinf,
		LrinfRrinfLrinf, LrinfRrinfRuninf, LrinfRrinfRlinf, LrinfRrinfRrinf,
		RuninfLuninfRuninf, RuninfLuninfRlinf, RuninfLuninfRrinf,
		RuninfLlinfRuninf, RuninfLlinfRlinf, RuninfLlinfRrinf,
		RuninfLrinfRuninf, RuninfLrinfRlinf, RuninfLrinfRrinf,
		RuninfRuninfRuninf, RuninfRuninfRlinf, RuninfRuninfRrinf,
		RuninfRlinfRuninf, RuninfRlinfRlinf, RuninfRlinfRrinf,
		RuninfRrinfRuninf, RuninfRrinfRlinf, RuninfRrinfRrinf,
		RlinfLuninfRlinf, RlinfLuninfRrinf,
		RlinfLlinfRlinf, RlinfLlinfRrinf,
		RlinfLrinfRlinf, RlinfLrinfRrinf,
		RlinfRuninfRlinf, RlinfRuninfRrinf,
		RlinfRlinfRlinf, RlinfRlinfRrinf,
		RlinfRrinfRlinf, RlinfRrinfRrinf,
		RrinfLuninfRrinf,
		RrinfLlinfRrinf,
		RrinfLrinfRrinf,
		RrinfRuninfRrinf,
		RrinfRlinfRrinf,
		RrinfRrinfRrinf
	};

	struct Params
	{
		double q; ///< spontaneous uninformed switching
		double QL; ///< spontaneous left informed switching
		double QR; ///< spontaneous right informed switching
		double w2; ///< one-on-one persuasion
		double w3; ///< symmetric triple persuasion
		double ad; ///< LR link creation
		double dd; ///< LR link destruction
		double ae; ///< LL/RR link creation
		double de; ///> LL/RR link destruction
	};

	SwarmModel(lnet::TripleNetwork& net, Params par);
	void setParams(Params par);
	double step(double currentTime);
	const event_list& switchEvents() const;
	const align_event_list& alignEvents() const;

private:
	void persuasion(TripleState s, double t);
	void conviction(LinkState s, double t);
	void switchState(NodeState s, double t);
	void createLink(NodeState a, NodeState b);
	void destroyLink(LinkState s);

	lnet::TripleNetwork& net_;
	Params par_;
	event_list switchEvents_;
	align_event_list alignEvents_;
};

inline void SwarmModel::setParams(const Params par)
{
	par_ = par;
}

inline const event_list& SwarmModel::switchEvents() const
{
	return switchEvents_;
}

inline const align_event_list& SwarmModel::alignEvents() const
{
	return alignEvents_;
}

#endif /* SWARMMODEL_H_ */
