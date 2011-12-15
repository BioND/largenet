/**
 * @file TimeSeriesOutput.cpp
 * @date 29.07.2009
 * @author gerd
 */

#include "TimeSeriesOutput.h"
#include <largenet/base/state_calculators.h>
#include <iomanip>

using namespace lnet;
using motifs::NodeMotifSet;
using motifs::LinkMotifSet;
using motifs::TripleMotifSet;

TimeSeriesOutput::TimeSeriesOutput(std::ostream& out, const TripleNetwork& net,
		const double interval) :
	IntervalOutput(out, interval), net_(net), nodeMotifs_(
			net.numberOfNodeStates()), linkMotifs_(net.numberOfNodeStates()),
			tripleMotifs_(net.numberOfNodeStates())
{
}

TimeSeriesOutput::~TimeSeriesOutput()
{
}

void TimeSeriesOutput::doOutput(const double t)
{
	const char sep = '\t';
	stream() << std::setprecision(9) << t;
	for (NodeMotifSet::const_iterator it = nodeMotifs_.begin(); it
			!= nodeMotifs_.end(); ++it)
		stream() << sep << net_.numberOfNodes(*it);
	for (LinkMotifSet::const_iterator it = linkMotifs_.begin(); it
			!= linkMotifs_.end(); ++it)
		stream() << sep << net_.numberOfLinks(net_.linkStateCalculator()(*it));
	/*for (TripleMotifSet::const_iterator it = tripleMotifs_.begin(); it
			!= tripleMotifs_.end(); ++it)
		stream() << sep << net_.numberOfTriples(net_.getTripleStateCalculator()(
				*it));*/
	stream() << "\n";
}

void TimeSeriesOutput::doWriteHeader()
{
	const char tab = '\t';
	stream() << commentChar() << 't';
	for (NodeMotifSet::const_iterator it = nodeMotifs_.begin(); it
			!= nodeMotifs_.end(); ++it)
		stream() << tab << *it;
	for (LinkMotifSet::const_iterator it = linkMotifs_.begin(); it
			!= linkMotifs_.end(); ++it)
		stream() << tab << *it;
	/*for (TripleMotifSet::const_iterator it = tripleMotifs_.begin(); it
				!= tripleMotifs_.end(); ++it)
			stream() << tab << *it;*/

	stream() << "\n";
}
