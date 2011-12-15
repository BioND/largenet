/**
 * @file DegDistOutput.h
 * @date 30.07.2009
 * @author gerd
 */

#ifndef DEGDISTOUTPUT_H_
#define DEGDISTOUTPUT_H_

#include "IntervalOutput.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>
#include <largenet/motifs/NodeMotif.h>

/**
 * Calculates and outputs the degree distribution for each node state in the network.
 */
class DegDistOutput: public IntervalOutput
{
public:
	DegDistOutput(std::ostream& out, const lnet::TripleNetwork& net, double interval);
	virtual ~DegDistOutput();
private:
	void doOutput(double t);
	void doWriteHeader();
	const lnet::TripleNetwork& net_;
	lnet::motifs::NodeMotifSet nodeMotifs_;
};

#endif /* DEGDISTOUTPUT_H_ */
