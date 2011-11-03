/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file QuadStarMotif.h
 * @date 14.12.2009
 * @author gerd
 */

#ifndef QUADSTARMOTIF_H_
#define QUADSTARMOTIF_H_

#include "../base/types.h"
#include <string>
#include <iostream>
#include <algorithm> // for std::swap

namespace lnet
{

namespace motifs
{

class QuadStarMotif
{
public:
	QuadStarMotif(node_state_t center, node_state_t a, node_state_t b, node_state_t c);
	std::string toStr() const;
	node_state_t center() const { return center_; }
	node_state_t a() const { return a_; }
	node_state_t b() const { return b_; }
	node_state_t c() const { return c_; }
	bool isSymmetric() const { return a_ == b_ && b_ == c_; }
	bool isMirrorSymmetric() const { return a_ == b_ || a_ == c_ || b_ == c_; }

private:
	node_state_t center_, a_, b_, c_;
};

bool operator==(const QuadStarMotif& A, const QuadStarMotif& B);
bool operator<(const QuadStarMotif& A, const QuadStarMotif& B);
std::ostream& operator<<(std::ostream& out, const QuadStarMotif& t);

inline QuadStarMotif::QuadStarMotif(const node_state_t center,
		const node_state_t a, const node_state_t b, const node_state_t c) :
	center_(center), a_(a), b_(b), c_(c)
{
	if (b_ < a_)
		std::swap(a_, b_);
	if (c_ < b_)
		std::swap(b_, c_);
	if (b_ < a_)
		std::swap(a_, b_);
}

}

}

#endif /* QUADSTARMOTIF_H_ */
