/**
 * @file TimeSeriesOutput.h
 * @date 29.07.2009
 * @author gerd
 */

#ifndef TIMESERIESOUTPUT_H_
#define TIMESERIESOUTPUT_H_

#include "IntervalOutput.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>
#include <largenet/motifs/NodeMotif.h>
#include <largenet/motifs/LinkMotif.h>
#include <largenet/motifs/TripleMotif.h>

/**
 * Outputs numbers of nodes in all states and numbers of links in all states in the network.
 */
class TimeSeriesOutput: public IntervalOutput
{
public:
	TimeSeriesOutput(std::ostream& out, const lnet::TripleNetwork& net,
			double interval);
	virtual ~TimeSeriesOutput();
private:
	void doOutput(double t);
	void doWriteHeader();
	const lnet::TripleNetwork& net_;
	const lnet::motifs::NodeMotifSet nodeMotifs_;
	const lnet::motifs::LinkMotifSet linkMotifs_;
	const lnet::motifs::TripleMotifSet tripleMotifs_;
};

#endif /* TIMESERIESOUTPUT_H_ */
