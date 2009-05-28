/*
 * Triple.h
 *
 *  Created on: 08.05.2009
 *      Author: gerd
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
