/*
 * graph_matrices.h
 *
 *  Created on: 13.02.2009
 *      Author: gerd
 */

#ifndef GRAPH_MATRICES_H_
#define GRAPH_MATRICES_H_

#include "../MultiNetwork.h"
#include <tnt/tnt_array2d.h>

namespace lnet
{

typedef TNT::Array2D<int> adjacency_matrix_t;

namespace spectral
{

adjacency_matrix_t adjacencyMatrix(const MultiNetwork& net);

}

}

#endif /* GRAPH_MATRICES_H_ */
