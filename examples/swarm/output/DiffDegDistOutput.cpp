/**
 * @file DiffDegDistOutput.cpp
 * @date 19.10.2009
 * @author gerd
 */

#include "DiffDegDistOutput.h"
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <iomanip>

using namespace std;
using namespace lnet;
using namespace boost::lambda;

DiffDegDistOutput::DiffDegDistOutput(ostream& out, const TripleNetwork& net,
		const double interval, const bool normalized) :
	IntervalOutput(out, interval), net_(net), nodeMotifs_(
			net_.numberOfNodeStates()), normalized_(normalized)
{
}

DiffDegDistOutput::~DiffDegDistOutput()
{
}

void DiffDegDistOutput::doWriteHeader()
{
	const char tab = '\t';
	stream() << commentChar();
	// degree distributions for each state
	for (motifs::NodeMotifSet::iterator motif = nodeMotifs_.begin(); motif
			!= nodeMotifs_.end(); ++motif)
	{
		stream() << tab << *motif;
	}

	for (motifs::NodeMotifSet::iterator node1 = nodeMotifs_.begin(); node1
			!= nodeMotifs_.end(); ++node1)
	{
		for (motifs::NodeMotifSet::iterator node2 = nodeMotifs_.begin(); node2
				!= nodeMotifs_.end(); ++node2)
		{
			stream() << tab << *node1 << "-" << *node2;
		}
	}
	stream() << "\n";
}

void DiffDegDistOutput::doOutput(const double t)
{
	stream() << commentChar() << "t = " << std::setprecision(9) << t << "\n";
	dist_vector dists;
	id_size_t maxDegree = 0;
	for (motifs::NodeMotifSet::iterator motif = nodeMotifs_.begin(); motif
			!= nodeMotifs_.end(); ++motif)
	{
		measures::degree_dist_t temp;
		measures::degreeDistribution(net_, temp, *motif);
		dists.push_back(temp);
		if (maxDegree < temp.size())
			maxDegree = temp.size();
	}

	for (motifs::NodeMotifSet::iterator node1 = nodeMotifs_.begin(); node1
			!= nodeMotifs_.end(); ++node1)
	{
		for (motifs::NodeMotifSet::iterator node2 = nodeMotifs_.begin(); node2
				!= nodeMotifs_.end(); ++node2)
		{
			measures::degree_dist_t temp;
			measures::degreeDistribution(net_, temp, *node1, *node2);
			dists.push_back(temp);
			if (maxDegree < temp.size())
				maxDegree = temp.size();
		}
	}
	if (normalized_)
		writeOutNormalized(dists, maxDegree);
	else
		writeOut(dists, maxDegree);
}

void DiffDegDistOutput::writeOut(const dist_vector& dists,
		const id_size_t maxDegree) const
{
	const char tab = '\t';
	for (id_size_t k = 0; k < maxDegree; ++k)
	{
		stream() << k;
		for (dist_vector::const_iterator i = dists.begin(); i != dists.end(); ++i)
		{
			if (k < i->size())
				stream() << tab << (*i)[k];
			else
				stream() << tab << 0;
		}
		stream() << "\n";
	}
	stream() << "\n\n";
}

void DiffDegDistOutput::writeOutNormalized(const dist_vector& dists,
		const id_size_t maxDegree) const
{
	typedef vector<vector<double> > dist_vector_d;
	dist_vector_d ndists(dists.size());
	for (size_t i = 0; i < ndists.size(); ++i)
		ndists[i].assign(dists[i].size(), 0);

	assert(ndists.size() == nodeMotifs_.size() * (nodeMotifs_.size() + 1));
	// normalize values
	size_t i = 0; // node motif index
	size_t s = nodeMotifs_.size();
	for (motifs::NodeMotifSet::iterator node = nodeMotifs_.begin(); node
			!= nodeMotifs_.end(); ++node)
	{
		std::transform(dists[i].begin(), dists[i].end(), ndists[i].begin(), _1
				/ static_cast<double> (net_.numberOfNodes(*node)));

		// dists should be {state0, state1, ..., stateN, state00, state01, ..., stateNN}
		for (size_t k = (i + 1) * s; k < (i + 2) * s; ++k)
		{
			std::transform(dists[k].begin(), dists[k].end(), ndists[k].begin(),
					_1 / static_cast<double> (net_.numberOfNodes(*node)));
		}
		++i;
	}

	const char tab = '\t';
	for (id_size_t k = 0; k < maxDegree; ++k)
	{
		stream() << k;
		for (dist_vector_d::iterator i = ndists.begin(); i != ndists.end(); ++i)
		{
			if (k < i->size())
				stream() << tab << (*i)[k];
			else
				stream() << tab << 0;
		}
		stream() << "\n";
	}
	stream() << "\n\n";
}
