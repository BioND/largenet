/*
 * TripleMotif.cpp
 *
 *  Created on: 22.01.2009
 *      Author: gerd
 */

#include "TripleMotif.h"
#include <sstream>

namespace lnet
{
namespace motifs {

std::string TripleMotif::toStr() const
{
	std::stringstream ss;
	ss << "(" << a_ << "," << b_ << "," << c_ << ")";
	return ss.str();
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
}
