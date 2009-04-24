/*
 * TripleMotif.cpp
 *
 *  Created on: 22.01.2009
 *      Author: gerd
 */

#include "TripleMotif.h"
#include <boost/format.hpp>
#include <cassert>

namespace lnet
{

TripleMotif::TripleMotif(const node_state_t aa, const node_state_t bb,
		const node_state_t cc) :
	a_(aa <= cc ? aa : cc), b_(bb), c_(aa <= cc ? cc : aa)
{
}

std::string TripleMotif::toStr() const
{
	return boost::str(boost::format("(%1%,%2%,%3%)") % a_ % b_ % c_);
}


bool operator==(const TripleMotif& A, const TripleMotif& B)
{
	return ((A.center() == B.center()) && (A.left() == B.left()) && (A.right()
			== B.right()));
}

bool operator<(const TripleMotif& A, const TripleMotif& B)
{
	if (A.left() < B.left())
		return true;
	else if (A.left() > B.left())
		return false;
	else if (A.center() < B.center())	// A.left() == B.left()
		return true;
	else if (A.center() > B.center())
		return false;
	else if (A.right() < B.right())		// A.center() == B.center()
		return true;
	else
		return false;
}

std::ostream& operator<<(std::ostream& out, const TripleMotif& t)
{
	return out << t.toStr();
}
}
