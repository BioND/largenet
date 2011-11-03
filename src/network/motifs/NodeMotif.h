/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef NODEMOTIF_H_
#define NODEMOTIF_H_

#include "../base/types.h"
#include <string>
#include <iostream>
#include <set>

namespace lnet
{
namespace motifs {

class NodeMotif
{
public:
	NodeMotif(node_state_t n);
	std::string toStr() const;
	operator node_state_t() const;
private:
	node_state_t n_;
};

bool operator==(const NodeMotif& A, const NodeMotif& B);
bool operator<(const NodeMotif& A, const NodeMotif& B);
std::ostream& operator<<(std::ostream& out, const NodeMotif& t);

inline NodeMotif::NodeMotif(const node_state_t n) : n_(n)
{
}

inline NodeMotif::operator node_state_t() const
{
	return n_;
}

}
}
#endif /* NODEMOTIF_H_ */
