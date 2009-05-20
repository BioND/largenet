/*
 * NodeMotif.h
 *
 *  Created on: 03.02.2009
 *      Author: gerd
 */

#ifndef NODEMOTIF_H_
#define NODEMOTIF_H_

#include "../base/types.h"
#include <string>
#include <iostream>
#include <set>

namespace lnet
{

class NodeMotif
{
public:
	explicit NodeMotif(node_state_t n);
	std::string toStr() const;
	node_state_t center() const;
	node_state_t operator()() const;
private:
	node_state_t n_;
};

bool operator==(const NodeMotif& A, const NodeMotif& B);
bool operator<(const NodeMotif& A, const NodeMotif& B);
std::ostream& operator<<(std::ostream& out, const NodeMotif& t);


inline node_state_t NodeMotif::center() const
{
	return n_;
}

inline node_state_t NodeMotif::operator()() const
{
	return n_;
}

}

#endif /* NODEMOTIF_H_ */
