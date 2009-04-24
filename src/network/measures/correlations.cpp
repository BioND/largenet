#include "correlations.h"

namespace lnet
{
namespace measures
{

double edgeDegreeCorrelationCoefficient(const MultiNetwork& net)
{
	id_size_t degree_product = 0, degree_sum = 0, square_degree_sum = 0;
	MultiNetwork::LinkIteratorRange iters = net.links();
	for (MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
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

TNT::Array2D<id_size_t> edgeDegreeCorrelationMatrix(const MultiNetwork& net)
{
	id_size_t max_deg = 0;
	MultiNetwork::NodeIteratorRange iters = net.nodes();
	for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
	{
		if (net.degree(*it) > max_deg)
			max_deg = net.degree(*it);
	}

	TNT::Array2D<id_size_t> temp(max_deg + 1, max_deg + 1,
			static_cast<id_size_t> (0));
	MultiNetwork::LinkIteratorRange liters = net.links();
	for (MultiNetwork::LinkIterator& lit = liters.first; lit != liters.second; ++lit)
	{
		++temp[net.degree(net.source(*lit))][net.degree(net.target(*lit))];
		++temp[net.degree(net.target(*lit))][net.degree(net.source(*lit))];
	}

	return temp;
}

}
}
