/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef MOTIFS_H_
#define MOTIFS_H_

#include <largenet/motifs/NodeMotif.h>
#include <largenet/motifs/LinkMotif.h>
#include <largenet/motifs/TripleMotif.h>
#include <largenet/motifs/QuadLineMotif.h>
#include <largenet/motifs/QuadStarMotif.h>
#include <largenet/motifs/MotifSet.h>

namespace lnet
{
namespace motifs
{
typedef MotifSet<NodeMotif> NodeMotifSet;
typedef MotifSet<LinkMotif> LinkMotifSet;
typedef MotifSet<TripleMotif> TripleMotifSet;
typedef MotifSet<QuadLineMotif> QuadLineMotifSet;
typedef MotifSet<QuadStarMotif> QuadStarMotifSet;
}
}

#endif /* MOTIFS_H_ */
