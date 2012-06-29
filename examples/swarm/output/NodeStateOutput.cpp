/**
 * @file NodeStateOutput.cpp
 * @date 12.10.2009
 * @author gerd
 */

#include "NodeStateOutput.h"
#include <iomanip>

using namespace lnet;

NodeStateOutput::NodeStateOutput(std::ostream& out, const TripleNetwork& net,
		const double interval) :
	IntervalOutput(out, interval), net_(net)
{
}

NodeStateOutput::~NodeStateOutput()
{
}

void NodeStateOutput::doWriteHeader()
{
	const char sep = '\t';
	stream() << commentChar() << 't';

	TripleNetwork::NodeIteratorRange iters = net_.nodes();
	for (TripleNetwork::NodeIterator it = iters.first; it != iters.second; ++it)
		stream() << sep << *it;
	stream() << '\n';
}

void NodeStateOutput::doOutput(const double t)
{
	const char sep = '\t';
	stream() << std::setprecision(9) << t;
	TripleNetwork::NodeIteratorRange iters = net_.nodes();
	for (TripleNetwork::NodeIterator it = iters.first; it != iters.second; ++it)
		stream() << sep << net_.nodeState(*it);
	stream() << '\n';
}
