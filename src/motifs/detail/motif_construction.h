/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef MOTIF_CONSTRUCTION_H_
#define MOTIF_CONSTRUCTION_H_

#include "../../network/types.h"

#include <set>

namespace lnet
{
namespace motifs
{
namespace detail
{

template<class Motif>
void constructAllMotifs(std::set<Motif>& m, node_state_size_t states);

}
}
}

#endif /* MOTIF_CONSTRUCTION_H_ */
