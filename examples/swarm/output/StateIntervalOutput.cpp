/**
 * @file StateIntervalOutput.cpp
 * @date 27.10.2009
 * @author gerd
 */

#include "StateIntervalOutput.h"
#include <cmath>
#include <iomanip>

StateIntervalOutput::StateIntervalOutput(std::ostream& out,
		const lnet::BasicNetwork& net, const lnet::node_state_t trackState,
		const double corridor, const double interval) :
	IntervalOutput(out, interval), trackState_(trackState), net_(net),
			lowThreshold_(static_cast<lnet::id_size_t> (round(0.5 * (1.0
					- corridor) * net_.numberOfNodes()))), highThreshold_(
					static_cast<lnet::id_size_t> (round(0.5 * (1.0 + corridor)
							* net_.numberOfNodes()))), above_(
					net_.numberOfNodes(trackState_) > highThreshold_), below_(
					net_.numberOfNodes(trackState_) < lowThreshold_),
			lastSwitch_(0)
{
	assert(corridor >= 0.0);
	assert(corridor < 1.0);
}

StateIntervalOutput::~StateIntervalOutput()
{
}

void StateIntervalOutput::doOutput(const double t)
{
	if (above_)
	{
		// was above upper threshold up to now
		if (net_.numberOfNodes(trackState_) <= highThreshold_)
		{
			// now below threshold == end of relevant period
			stream() << std::setprecision(9) << t << '\t' << t - lastSwitch_
					<< '\n';
			lastSwitch_ = t; // in case we jump to other side immediately
			above_ = false;
			below_ = net_.numberOfNodes(trackState_) < lowThreshold_;
		}
	}
	else if (below_)
	{
		// was below lower threshold up to now
		if (net_.numberOfNodes(trackState_) >= lowThreshold_)
		{
			// now above threshold == end of relevant period
			stream() << std::setprecision(9) << t << '\t' << t - lastSwitch_
					<< '\n';
			lastSwitch_ = t; // in case we jump to other side immediately
			below_ = false;
			above_ = net_.numberOfNodes(trackState_) > highThreshold_;
		}
	}
	else
	{
		// was inside corridor up to now
		if (net_.numberOfNodes(trackState_) > highThreshold_
				|| net_.numberOfNodes(trackState_) < lowThreshold_)
		{
			// now outside corridor == start of relevant period
			lastSwitch_ = t;
			above_ = net_.numberOfNodes(trackState_) > highThreshold_;
			below_ = net_.numberOfNodes(trackState_) < lowThreshold_;
		}
	}
}

void StateIntervalOutput::doWriteHeader()
{
	stream() << commentChar() << " Lower threshold: " << lowThreshold_ << " ("
			<< static_cast<double> (lowThreshold_) / net_.numberOfNodes()
			<< ")\n" << commentChar() << " Upper threshold: " << highThreshold_
			<< " (" << static_cast<double> (highThreshold_)
			/ net_.numberOfNodes() << ")\n" << commentChar() << "t\tdt\n";
}
