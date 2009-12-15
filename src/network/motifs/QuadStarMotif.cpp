/**
 * @file QuadStarMotif.cpp
 * @date 14.12.2009
 * @author gerd
 */

#include "QuadStarMotif.h"
#include <sstream>

namespace lnet
{

namespace motifs
{

std::string QuadStarMotif::toStr() const
{
	std::stringstream ss;
	ss << "(" << a_ << "-" << center_ << "<" << b_ << "," << c_ << ")";
	return ss.str();
}

bool operator==(const QuadStarMotif& A, const QuadStarMotif& B)
{
	return ((A.center() == B.center()) && (A.a() == B.a()) && (A.b() == B.b())
			&& (A.c() == B.c()));
}

bool operator<(const QuadStarMotif& A, const QuadStarMotif& B)
{
	if (A.center() < B.center())
		return true;
	else if (A.a() < B.a()) // A.center() == B.center()
		return true;
	else if (A.a() > B.a())
		return false;
	else if (A.b() < B.b()) // A.a() == B.a()
		return true;
	else if (A.b() > B.b())
		return false;
	else if (A.c() < B.c()) // A.b() == B.b()
		return true;
	else return false;
}

std::ostream& operator<<(std::ostream& out, const QuadStarMotif& t)
{
	return out << t.toStr();
}
}

}
