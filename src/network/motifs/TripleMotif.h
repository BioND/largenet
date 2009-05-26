/*
 * TripleMotif.h
 *
 *  Created on: 22.01.2009
 *      Author: gerd
 */

#ifndef TRIPLEMOTIF_H_
#define TRIPLEMOTIF_H_

#include "../base/types.h"
#include <string>
#include <iostream>

namespace lnet
{
namespace motifs {

/**
 * An undirected triple. State fields @p a, @p b, @p c are always ordered.
 */
class TripleMotif
{
public:
	TripleMotif(node_state_t aa, node_state_t bb, node_state_t cc);
	std::string toStr() const;
	node_state_t left() const;
	node_state_t center() const;
	node_state_t right() const;
	bool isSymmetric() const;
private:
	node_state_t a_, b_, c_;
};

bool operator==(const TripleMotif& A, const TripleMotif& B);
bool operator<(const TripleMotif& A, const TripleMotif& B);
std::ostream& operator<<(std::ostream& out, const TripleMotif& t);

inline node_state_t TripleMotif::left() const
{
	return a_;
}

inline node_state_t TripleMotif::center() const
{
	return b_;
}

inline node_state_t TripleMotif::right() const
{
	return c_;
}

inline bool TripleMotif::isSymmetric() const
{
	return a_ == c_;
}
}
}
#endif /* TRIPLEMOTIF_H_ */
