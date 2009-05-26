/*
 * NodeMotif.cpp
 *
 *  Created on: 03.02.2009
 *      Author: gerd
 */

#include "NodeMotif.h"
#include <boost/format.hpp>

namespace lnet
{
namespace motifs {

NodeMotif::NodeMotif(const node_state_t n) :
	n_(n)
{
}

std::string NodeMotif::toStr() const
{
	return boost::str(boost::format("(%1%)") % n_);
}

bool operator==(const NodeMotif& A, const NodeMotif& B)
{
	return A() == B();
}

bool operator<(const NodeMotif& A, const NodeMotif& B)
{
	return A() < B();
}

std::ostream& operator<<(std::ostream& out, const NodeMotif& n)
{
	return out << n.toStr();
}

}
}
