/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file correlations.h
 *
 *  Created on: 13.03.2009
 *      Author: gerd
 */

#ifndef CORRELATIONS_H_
#define CORRELATIONS_H_

#include "../../largenet.h"
#include "../tnt/tnt_array2d.h"

namespace lnet {
namespace measures {

/**
 * Pearson degree correlation coefficient of the degrees at either ends of a link.
 * @see M. Newman: Assortative Mixing in Networks, Phys. Rev. Lett. 89, 208701
 * @param net MultiNetwork to calculate correlation coefficient for.
 * @return degree correlation coefficient
 */
double edgeDegreeCorrelationCoefficient(const BasicNetwork& net);

TNT::Array2D<id_size_t> edgeDegreeCorrelationMatrix(const BasicNetwork& net);

}
}


#endif /* CORRELATIONS_H_ */
