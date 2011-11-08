/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#include "NodeMotif.h"
#include <sstream>

namespace lnet
{
namespace motifs
{

std::string NodeMotif::toStr() const
{
	std::stringstream ss;
	ss << "(" << n_ << ")";
	return ss.str();
}

bool operator==(const NodeMotif& A, const NodeMotif& B)
{
	return static_cast<node_state_t> (A) == static_cast<node_state_t> (B);
}

bool operator<(const NodeMotif& A, const NodeMotif& B)
{
	return static_cast<node_state_t> (A) < static_cast<node_state_t> (B);
}

std::ostream& operator<<(std::ostream& out, const NodeMotif& n)
{
	return out << n.toStr();
}

}
}
