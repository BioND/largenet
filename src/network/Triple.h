/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef TRIPLE_H_
#define TRIPLE_H_

#include "types.h"

namespace lnet
{

class Triple
{
public:
	Triple();
	Triple(link_id_t left, link_id_t right);
	link_id_t left() const;
	link_id_t right() const;
	void setLeft(link_id_t left);
	void setRight(link_id_t right);
	bool containsLink(link_id_t l) const;

private:
	link_id_t left_, right_;
};

inline Triple::Triple() : left_(0), right_(0)
{
}

inline Triple::Triple(const link_id_t left, const link_id_t right) :
	left_(left), right_(right)
{
}

inline link_id_t Triple::left() const
{
	return left_;
}

inline link_id_t Triple::right() const
{
	return right_;
}

inline void Triple::setLeft(const link_id_t left)
{
	left_ = left;
}

inline void Triple::setRight(const link_id_t right)
{
	right_ = right;
}

inline bool Triple::containsLink(const link_id_t l) const
{
	return (l == right_) || (l == left_);
}

}

#endif /* TRIPLE_H_ */
