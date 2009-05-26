#include "measures.h"
#include "../TripleMultiNetwork.h"
#include "../TripleNetwork.h"

namespace lnet
{
namespace measures
{
/*
 void degreeDistribution(const MultiNetwork& net, degree_dist_t& dist)
 {
 dist.clear();
 MultiNetwork::NodeIteratorRange iters = net.nodes();
 for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
 {
 id_size_t deg = net.degree(*it);
 while (deg >= dist.size())
 {
 dist.push_back(0);
 }
 ++dist[deg];
 }
 }

 void degreeDistribution(const MultiNetwork& net, degree_dist_t& dist,
 const node_state_t s)
 {
 dist.clear();
 MultiNetwork::NodeStateIteratorRange iters = net.nodes(s);
 for (MultiNetwork::NodeStateIterator& it = iters.first; it != iters.second; ++it)
 {
 id_size_t deg = net.degree(*it);
 while (deg >= dist.size())
 {
 dist.push_back(0);
 }
 ++dist[deg];
 }
 }

 void degreeDistribution(const MultiNetwork& net, degree_dist_t& dist,
 node_state_t source, node_state_t target)
 {
 dist.clear();
 MultiNetwork::NodeStateIteratorRange iters = net.nodes(source);
 for (MultiNetwork::NodeStateIterator& it = iters.first; it != iters.second; ++it)
 {
 MultiNetwork::NeighborIteratorRange neighbors = net.neighbors(*it);
 id_size_t deg = 0;
 for (MultiNetwork::NeighborIterator& nit = neighbors.first; nit
 != neighbors.second; ++nit)
 {
 if (net.getNodeState(*nit) == target)
 ++deg;
 }

 while (deg >= dist.size())
 {
 dist.push_back(0);
 }
 ++dist[deg];
 }
 }

 void averageNearestNeighborDegree(const MultiNetwork& net,
 std::vector<double>& dist)
 {
 degree_dist_t degdist;

 // FIXME For better performance, it might be a good idea to calculate this on the fly.
 degreeDistribution(net, degdist);
 std::vector<double> avgNeighborDegrees(degdist.size());

 MultiNetwork::NodeIteratorRange iters = net.nodes();
 for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
 {
 id_size_t sum = 0, deg = net.degree(*it);
 MultiNetwork::NeighborIteratorRange neighbors = net.neighbors(*it);
 for (MultiNetwork::NeighborIterator& nit = neighbors.first; nit
 != neighbors.second; ++nit)
 {
 sum += net.degree(*nit);
 }
 while (deg >= avgNeighborDegrees.size())
 {
 avgNeighborDegrees.push_back(0);
 }
 if (deg > 0)
 avgNeighborDegrees[deg] += static_cast<double> (sum) / deg;
 }

 assert(degdist.size() == avgNeighborDegrees.size());
 dist.clear();
 dist.assign(degdist.size(), 0);
 for (unsigned int i = 0; i < degdist.size(); ++i)
 {
 if (degdist[i] != 0)
 dist[i] = avgNeighborDegrees[i] / degdist[i];
 }
 }

 void averageNearestNeighborDegree(const MultiNetwork& net,
 std::vector<double>& dist, const node_state_t state)
 {
 degree_dist_t degdist;

 // FIXME For better performance, it might be a good idea to calculate this on the fly.
 degreeDistribution(net, degdist, state);

 std::vector<double> avgNeighborDegrees(degdist.size());

 MultiNetwork::NodeStateIteratorRange iters = net.nodes(state);
 for (MultiNetwork::NodeStateIterator& it = iters.first; it != iters.second; ++it)
 {
 id_size_t sum = 0, deg = net.degree(*it);
 MultiNetwork::NeighborIteratorRange neighbors = net.neighbors(*it);
 for (MultiNetwork::NeighborIterator& nit = neighbors.first; nit
 != neighbors.second; ++nit)
 {
 sum += net.degree(*nit);
 }
 while (deg >= avgNeighborDegrees.size())
 {
 avgNeighborDegrees.push_back(0);
 }
 if (deg > 0)
 avgNeighborDegrees[deg] += static_cast<double> (sum) / deg;
 }

 assert(degdist.size() == avgNeighborDegrees.size());
 dist.clear();
 dist.assign(degdist.size(), 0);
 for (unsigned int i = 0; i < degdist.size(); ++i)
 {
 if (degdist[i] != 0)
 dist[i] = avgNeighborDegrees[i] / degdist[i];
 }
 }

 double clusteringCoefficient(const MultiNetwork& net, const node_id_t n)
 {
 const id_size_t deg = net.degree(n);
 if (deg <= 1)
 return 0;

 id_size_t triangles = 0;

 MultiNetwork::NeighborIteratorRange neighbors = net.neighbors(n);
 for (MultiNetwork::NeighborIterator it1 = neighbors.first; it1
 != neighbors.second; ++it1)
 {
 for (MultiNetwork::NeighborIterator it2 = neighbors.first; it2 != it1; ++it2)
 // this should be different for directed networks
 {
 if (net.isLink(*it1, *it2).first)
 ++triangles;
 }
 }
 return static_cast<double> (triangles) * 6 / (deg * (deg - 1));
 }

 double clusteringCoefficient(const MultiNetwork& net)
 {
 if (net.numberOfNodes() == 0)
 return 0;
 double sum = 0;
 MultiNetwork::NodeIteratorRange iters = net.nodes();
 for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
 {
 sum += clusteringCoefficient(net, *it);
 }
 return sum / net.numberOfNodes();
 }

 double transitivity(const MultiNetwork& net)
 {
 unsigned int triangles = 0, contri = 0;
 MultiNetwork::NodeIteratorRange iters = net.nodes();
 for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
 {
 id_size_t deg = net.degree(*it);
 contri += deg * (deg - 1); // twice the number of possible triangles
 MultiNetwork::NeighborIteratorRange niters = net.neighbors(*it);
 for (MultiNetwork::NeighborIterator nit1 = niters.first; nit1
 != niters.second; ++nit1)
 {
 for (MultiNetwork::NeighborIterator nit2 = niters.first; nit2
 != nit1; ++nit2)
 {
 if (net.isLink(*nit1, *nit2).first)
 ++triangles;
 }
 }
 }
 if (contri == 0)
 return 0;
 else
 return 2.0 * triangles / static_cast<double> (contri);
 }

 id_size_t triples_slow(const MultiNetwork& net)
 {
 id_size_t t = 0;
 MultiNetwork::LinkIteratorRange iters = net.links();
 for (MultiNetwork::LinkIterator& it = iters.first; it != iters.second; ++it)
 {
 t += net.degree(net.target(*it)) + net.degree(net.source(*it)) - 2;
 }
 return t / 2;
 }

 id_size_t triples(const MultiNetwork& net)
 {
 id_size_t t = 0;
 MultiNetwork::NodeIteratorRange iters = net.nodes();
 for (MultiNetwork::NodeIterator& it = iters.first; it != iters.second; ++it)
 {
 const id_size_t d = net.degree(*it);
 if (d > 0)
 t += d * (d - 1);
 }
 return t / 2;
 }
 */

template<>
id_size_t triples<TripleMultiNetwork> (const TripleMultiNetwork& net)
{
	return net.numberOfTriples();
}

template<>
id_size_t triples<TripleNetwork> (const TripleNetwork& net)
{
	return net.numberOfTriples();
}

template<>
id_size_t triples<TripleMultiNetwork> (const TripleMultiNetwork& net,
		const motifs::TripleMotif& t)
{
	return net.numberOfTriples(net.getTripleStateCalculator()(t.left(),
			t.center(), t.right()));
}

template<>
id_size_t triples<TripleNetwork> (const TripleNetwork& net,
		const motifs::TripleMotif& t)
{
	return net.numberOfTriples(net.getTripleStateCalculator()(t.left(),
			t.center(), t.right()));
}

}
}
