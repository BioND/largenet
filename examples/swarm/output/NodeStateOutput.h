/**
 * @file NodeStateOutput.h
 * @date 12.10.2009
 * @author gerd
 */

#ifndef NODESTATEOUTPUT_H_
#define NODESTATEOUTPUT_H_

#include "IntervalOutput.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>

class NodeStateOutput: public IntervalOutput
{
public:
	NodeStateOutput(std::ostream& out, const lnet::TripleNetwork& net, double interval);
	virtual ~NodeStateOutput();
private:
	void doOutput(double t);
	void doWriteHeader();
	const lnet::TripleNetwork& net_;
};

#endif /* NODESTATEOUTPUT_H_ */
