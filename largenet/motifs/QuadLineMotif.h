/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef QUADLINEMOTIF_H_
#define QUADLINEMOTIF_H_

#include "../base/types.h"
#include <string>
#include <iostream>
#include <algorithm> // for std::swap

namespace lnet
{

namespace motifs
{

class QuadLineMotif
{
public:
	QuadLineMotif(node_state_t aa, node_state_t bb, node_state_t cc, node_state_t dd);
	std::string toStr() const;
	node_state_t a() const { return a_; }
	node_state_t b() const { return b_; }
	node_state_t c() const { return c_; }
	node_state_t d() const { return d_; }
	bool isSymmetric() const { return a_ == d_ && b_ == c_; }
private:
	node_state_t a_, b_, c_, d_;
};

bool operator==(const QuadLineMotif& A, const QuadLineMotif& B);
bool operator<(const QuadLineMotif& A, const QuadLineMotif& B);
std::ostream& operator<<(std::ostream& out, const QuadLineMotif& t);

inline QuadLineMotif::QuadLineMotif(const node_state_t aa,
		const node_state_t bb, const node_state_t cc, const node_state_t dd) :
	a_(aa <= dd ? aa : dd), b_(aa <= dd ? bb : cc),
	c_(aa <= dd ? cc : bb), d_(aa <= dd ? dd : aa)
{
	if ((a_ == d_) && (b_ > c_))
	{
		std::swap(a_, d_);
		std::swap(b_, c_);
	}
}

}

}

#endif /* QUADLINEMOTIF_H_ */
