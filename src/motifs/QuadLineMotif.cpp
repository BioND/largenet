/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
#include "QuadLineMotif.h"
#include <sstream>

namespace lnet
{
namespace motifs
{

std::string QuadLineMotif::toStr() const
{
	std::stringstream ss;
	ss << "(" << a_ << "," << b_ << "," << c_ << "," << d_ << ")";
	return ss.str();
}

bool operator==(const QuadLineMotif& A, const QuadLineMotif& B)
{
	return ((A.a() == B.a()) && (A.b() == B.b()) && (A.c() == B.c()) && (A.d()
			== B.d()));
}

bool operator<(const QuadLineMotif& A, const QuadLineMotif& B)
{
	if (A.a() < B.a())
		return true;
	else if (A.a() > B.a())
		return false;
	else if (A.b() < B.b()) // A.a() == B.a()
		return true;
	else if (A.b() > B.b())
		return false;
	else if (A.c() < B.c()) // A.b() == B.b()
		return true;
	else if (A.c() > B.c())
		return false;
	else if (A.d() < B.d()) // A.c() == B.c()
		return true;
	else
		return false;
}

std::ostream& operator<<(std::ostream& out, const QuadLineMotif& t)
{
	return out << t.toStr();
}
}
}
