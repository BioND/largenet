/**
 * @file DiffDegDistOutput.h
 * @date 19.10.2009
 * @author gerd
 */

#ifndef DIFFDEGDISTOUTPUT_H_
#define DIFFDEGDISTOUTPUT_H_

#include "IntervalOutput.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>
#include <largenet/motifs/NodeMotif.h>
#include <largenet/measures/measures.h>
#include <vector>

/**
 * Calculates and outputs the degree distributions and differential degree distributions for each node state in the network.
 */
class DiffDegDistOutput: public IntervalOutput
{
public:
	DiffDegDistOutput(std::ostream& out, const lnet::TripleNetwork& net, double interval, bool normalize=false);
	virtual ~DiffDegDistOutput();
private:
	typedef std::vector<lnet::measures::degree_dist_t> dist_vector;
	void doOutput(double t);
	void doWriteHeader();
	void writeOut(const dist_vector& dists, lnet::id_size_t maxDegree) const;
	void writeOutNormalized(const dist_vector& dists, lnet::id_size_t maxDegree) const;
	const lnet::TripleNetwork& net_;
	lnet::motifs::NodeMotifSet nodeMotifs_;
	bool normalized_;
};

#endif /* DIFFDEGDISTOUTPUT_H_ */
