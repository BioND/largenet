/*
 * motifs.h
 *
 *  Created on: 03.02.2009
 *      Author: gerd
 */

#ifndef MOTIFS_H_
#define MOTIFS_H_

#include "NodeMotif.h"
#include "LinkMotif.h"
#include "TripleMotif.h"
#include "QuadLineMotif.h"
#include "MotifSet.h"

namespace lnet
{
namespace motifs
{
typedef MotifSet<NodeMotif> NodeMotifSet;
typedef MotifSet<LinkMotif> LinkMotifSet;
typedef MotifSet<TripleMotif> TripleMotifSet;
typedef MotifSet<QuadLineMotif> QuadLineMotifSet;
}
}

#endif /* MOTIFS_H_ */
