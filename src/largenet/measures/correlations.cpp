/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
#include "correlations.h"

namespace lnet
{
namespace measures
{

double edgeDegreeCorrelationCoefficient(const BasicNetwork& net)
{
	id_size_t degree_product = 0, degree_sum = 0, square_degree_sum = 0;
	BasicNetwork::LinkIteratorRange iters = net.links();
	for (BasicNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
	{
		id_size_t ks = net.degree(net.source(*it)), kt = net.degree(net.target(
				*it));
		degree_product += ks * kt;
		degree_sum += ks + kt;
		square_degree_sum += ks * ks + kt * kt;
	}

	double b = 0.25 * degree_sum * degree_sum
			/ static_cast<double> (net.numberOfLinks());
	return (degree_product - b) / (0.5
			* static_cast<double> (square_degree_sum) - b);
}

TNT::Array2D<id_size_t> edgeDegreeCorrelationMatrix(const BasicNetwork& net)
{
	id_size_t max_deg = 0;
	BasicNetwork::NodeIteratorRange iters = net.nodes();
	for (BasicNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		if (net.degree(*it) > max_deg)
			max_deg = net.degree(*it);
	}

	TNT::Array2D<id_size_t> temp(max_deg + 1, max_deg + 1,
			static_cast<id_size_t> (0));
	BasicNetwork::LinkIteratorRange liters = net.links();
	for (BasicNetwork::LinkIterator& lit = liters.first; lit != liters.second; ++lit)
	{
		++temp[net.degree(net.source(*lit))][net.degree(net.target(*lit))];
		++temp[net.degree(net.target(*lit))][net.degree(net.source(*lit))];
	}

	return temp;
}

}
}
