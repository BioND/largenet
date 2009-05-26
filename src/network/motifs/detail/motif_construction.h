/*
 * motif_construction.h
 *
 *  Created on: 03.02.2009
 *      Author: gerd
 */

#ifndef MOTIF_CONSTRUCTION_H_
#define MOTIF_CONSTRUCTION_H_

#include "../../base/types.h"
#include "../NodeMotif.h"
#include "../LinkMotif.h"
#include "../TripleMotif.h"
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
