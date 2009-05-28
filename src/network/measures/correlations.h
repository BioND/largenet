/**
 * @file correlations.h
 *
 *  Created on: 13.03.2009
 *      Author: gerd
 */

#ifndef CORRELATIONS_H_
#define CORRELATIONS_H_

#include "../largenet.h"
#include "../../tnt/tnt_array2d.h"

namespace lnet {
namespace measures {

/**
 * Pearson degree correlation coefficient of the degrees at either ends of a link.
 * @see M. Newman: Assortative Mixing in Networks, Phys. Rev. Lett. 89, 208701
 * @param net MultiNetwork to calculate correlation coefficient for.
 * @return degree correlation coefficient
 */
double edgeDegreeCorrelationCoefficient(const MultiNetwork& net);

TNT::Array2D<id_size_t> edgeDegreeCorrelationMatrix(const MultiNetwork& net);

}
}


#endif /* CORRELATIONS_H_ */
