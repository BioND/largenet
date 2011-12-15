/**
 * @file DegDistOutput.cpp
 * @date 30.07.2009
 * @author gerd
 */

#include "DegDistOutput.h"
#include <largenet/measures/measures.h>
#include <largenet/motifs/NodeMotif.h>
#include <largenet/motifs/LinkMotif.h>
#include <largenet/motifs/TripleMotif.h>
#include <iomanip>
#include <map>

using namespace std;
using namespace lnet;

DegDistOutput::DegDistOutput(ostream& out, const TripleNetwork& net,
		const double interval) :
	IntervalOutput(out, interval), net_(net), nodeMotifs_(
			net_.numberOfNodeStates())
{
}

DegDistOutput::~DegDistOutput()
{
}

void DegDistOutput::doWriteHeader()
{
	const char tab = '\t';
	stream() << commentChar();
	for (motifs::NodeMotifSet::const_iterator motif = nodeMotifs_.begin(); motif
			!= nodeMotifs_.end(); ++motif)
	{
		stream() << tab << *motif;
	}
	stream() << "\n";
}

void DegDistOutput::doOutput(const double t)
{
	stream() << commentChar() << "t = " << std::setprecision(9) << t << "\n";
	map<motifs::NodeMotif, measures::degree_dist_t> dists;
	id_size_t maxDegree = 0;
	for (motifs::NodeMotifSet::const_iterator motif = nodeMotifs_.begin(); motif
			!= nodeMotifs_.end(); ++motif)
	{
		measures::degreeDistribution(net_, dists[*motif], *motif);
		if (maxDegree < dists[*motif].size())
			maxDegree = dists[*motif].size();
	}

	const char tab = '\t';
	for (id_size_t k = 0; k < maxDegree; ++k)
	{
		stream() << k;
		for (motifs::NodeMotifSet::const_iterator motif = nodeMotifs_.begin(); motif
				!= nodeMotifs_.end(); ++motif)
		{
			if (k < dists[*motif].size())
				stream() << tab << dists[*motif][k];
			else
				stream() << tab << 0;
		}
		stream() << "\n";
	}
	stream() << "\n\n";
}
