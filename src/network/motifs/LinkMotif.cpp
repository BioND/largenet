/*
 * LinkMotif.cpp
 *
 *  Created on: 03.02.2009
 *      Author: gerd
 */

#include "LinkMotif.h"
#include <sstream>

namespace lnet
{
namespace motifs
{

LinkMotif::LinkMotif(const node_state_t aa, const node_state_t bb) :
	a_(aa <= bb ? aa : bb), b_(aa <= bb ? bb : aa)
{
}

std::string LinkMotif::toStr() const
{
	std::stringstream ss;
	ss << "(" << a_ << "," << b_ << ")";
	return ss.str();
}

bool operator==(const LinkMotif& A, const LinkMotif& B)
{
	return (A.left() == B.left()) && (A.right() == B.right());
}

bool operator<(const LinkMotif& A, const LinkMotif& B)
{
	if (A.left() < B.left())
		return true;
	else if (A.left() > B.left())
		return false;
	else if (A.right() < B.right())
		return true;
	else
		return false;
}

std::ostream& operator<<(std::ostream& out, const LinkMotif& l)
{
	return out << l.toStr();
}
}
}
