/*
 * LinkMotif.h
 *
 *  Created on: 03.02.2009
 *      Author: gerd
 */

#ifndef LINKMOTIF_H_
#define LINKMOTIF_H_

#include "../base/types.h"
#include <string>
#include <iostream>
//#include <set>

namespace lnet
{
namespace motifs {

/**
 * An undirected link. State fields @p a, @p b are always ordered.
 */
class LinkMotif
{
public:
	LinkMotif(node_state_t aa, node_state_t bb);
	std::string toStr() const;
	node_state_t left() const;
	node_state_t right() const;
	bool isSymmetric() const;
private:
	node_state_t a_, b_;
};

bool operator==(const LinkMotif& A, const LinkMotif& B);
bool operator<(const LinkMotif& A, const LinkMotif& B);
std::ostream& operator<<(std::ostream& out, const LinkMotif& t);

inline node_state_t LinkMotif::left() const
{
	return a_;
}

inline node_state_t LinkMotif::right() const
{
	return b_;
}

inline bool LinkMotif::isSymmetric() const
{
	return a_ == b_;
}
}
}
#endif /* LINKMOTIF_H_ */
