/* Keeps log of the state alignment events
 * Info for the time of the alignment, the new state assumed,
 * whether the population is in the majority side and whether alignment results from an interaction with an informed
 */

#ifndef STATEALIGNEVENT_H_
#define STATEALIGNEVENT_H_

#include <largenet.h>
#include <largenet/base/types.h>

class StateAlignEvent
{
public:
	StateAlignEvent(double t, lnet::node_state_t newState, double maj, bool inf) :
		time_(t), newState_(newState), maj_(maj), inf_(inf)
	{
	}

	StateAlignEvent(const StateAlignEvent& o) :
		time_(o.time_), newState_(o.newState_), maj_(o.maj_), inf_(o.inf_)
	{
	}

	StateAlignEvent& operator=(const StateAlignEvent& o)
	{
		if (this != &o)
		{
			time_ = o.time_;
			newState_ = o.newState_;
			maj_ = o.maj_;
			inf_ = o.inf_;
		}
		return *this;
	}

	double time() const
	{
		return time_;
	}
	double maj() const
	{
		return maj_;
	}
	bool inf() const
	{
		return inf_;
	}
	lnet::node_state_t newState() const
	{
		return newState_;
	}

private:
	double time_;
	double maj_;
	bool inf_;
	lnet::node_state_t newState_;
};

#endif /* STATEALIGNEVENT_H_ */
