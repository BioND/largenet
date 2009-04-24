/*
 * graph_matrices.cpp
 *
 *  Created on: 13.02.2009
 *      Author: gerd
 */

#include "graph_matrices.h"
#include <tnt/jama_eig.h>

namespace lnet
{
namespace spectral
{

using namespace TNT;

adjacency_matrix_t adjacencyMatrix(const MultiNetwork& net)
{
	adjacency_matrix_t temp(net.numberOfNodes(), net.numberOfNodes(), 0);
	MultiNetwork::LinkIteratorRange iters = net.links();
	for (MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
	{
		temp[net.source(*it)][net.target(*it)] = 1;
	}
	return temp; // this should work, since TNT matrices have implicit sharing (ref-counted) data
}

}
}
