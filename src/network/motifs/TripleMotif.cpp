/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
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
