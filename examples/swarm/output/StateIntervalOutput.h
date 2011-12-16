/**
 * @file StateIntervalOutput.h
 * @date 27.10.2009
 * @author gerd
 */

#ifndef STATEINTERVALOUTPUT_H_
#define STATEINTERVALOUTPUT_H_

#include "IntervalOutput.h"
#include <largenet.h>
#include <largenet/base/BasicNetwork.h>

class StateIntervalOutput: public IntervalOutput
{
public:
	StateIntervalOutput(std::ostream& out, const lnet::BasicNetwork& net,
			lnet::node_state_t trackState, double corridor, double interval = 0);
	virtual ~StateIntervalOutput();
private:
	void doOutput(double t);
	void doWriteHeader();
	const lnet::node_state_t trackState_;
	const lnet::BasicNetwork& net_;
	const lnet::id_size_t lowThreshold_, highThreshold_;
	bool above_, below_;
	double lastSwitch_;
};

#endif /* STATEINTERVALOUTPUT_H_ */
