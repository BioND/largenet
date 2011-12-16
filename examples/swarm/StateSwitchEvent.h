/* Keeps log of the state switching events
 * Info for the time of the alignment, the new state assumed, and whether the population is in the majority side.
 */

#ifndef STATESWITCHEVENT_H_
#define STATESWITCHEVENT_H_

#include <largenet.h>
#include <largenet/base/types.h>

class StateSwitchEvent
{
public:
	StateSwitchEvent(double t, lnet::node_state_t newState, double maj) :
		time_(t), newState_(newState), maj_(maj)
	{
	}

	StateSwitchEvent(const StateSwitchEvent& o) :
		time_(o.time_), newState_(o.newState_),maj_(o.maj_)
	{
	}

	StateSwitchEvent& operator=(const StateSwitchEvent& o)
	{
		if (this != &o)
		{
			time_ = o.time_;
			newState_ = o.newState_;
			maj_=o.maj_;
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
	lnet::node_state_t newState() const
	{
		return newState_;
	}

private:
	double time_;
	lnet::node_state_t newState_;
	double maj_;
};

#endif /* STATESWITCHEVENT_H_ */
