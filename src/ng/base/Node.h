/**
 * @file Node.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Node class.
 */
#ifndef NODE_H_
#define NODE_H_

#include "types.h"
#include "exceptions.h"
#include "node_iterators.h"
#include <set>
#include <utility>
#include <boost/noncopyable.hpp>

namespace largenet
{

class Edge;

class Node: public boost::noncopyable
{
	friend class Edge;
public:
	typedef std::set<Edge*> edge_set;
	typedef edge_set::iterator edge_iterator;
	typedef edge_set::const_iterator const_edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;

	typedef iterators::NodeOutNeighborIterator<edge_iterator>
			OutNeighborIterator;
	typedef iterators::NodeOutNeighborIterator<const_edge_iterator, true>
			ConstOutNeighborIterator;
	typedef iterators::NodeInNeighborIterator<edge_iterator> InNeighborIterator;
	typedef iterators::NodeInNeighborIterator<const_edge_iterator, true>
			ConstInNeighborIterator;

	typedef std::pair<OutNeighborIterator, OutNeighborIterator>
			OutNeighborIteratorRange;
	typedef std::pair<InNeighborIterator, InNeighborIterator>
			InNeighborIteratorRange;
	typedef std::pair<ConstOutNeighborIterator, ConstOutNeighborIterator>
			ConstOutNeighborIteratorRange;
	typedef std::pair<ConstInNeighborIterator, ConstInNeighborIterator>
			ConstInNeighborIteratorRange;

	Node(node_id_t id) :
		id_(id)
	{
	}
	virtual ~Node()
	{
	}
	node_id_t id() const
	{
		return id_;
	}
	virtual degree_size_t outDegree() const = 0;
	virtual degree_size_t inDegree() const = 0;
	degree_size_t degree() const
	{
		return outDegree() + inDegree();
	}
	virtual bool hasInEdge(Edge* e) const = 0;
	virtual bool hasOutEdge(Edge* e) const = 0;
	bool hasEdge(Edge* e) const
	{
		return hasOutEdge(e) || hasInEdge(e);
	}
	virtual bool hasEdgeTo(const Node* n) const = 0;
	virtual bool hasEdgeFrom(const Node* n) const = 0;
	virtual Edge* edgeTo(const Node* n) const = 0;
	virtual Edge* edgeFrom(const Node* n) const = 0;
	Edge* edgeToAdjacentNode(const Node* n) const
	{
		Edge* e = edgeTo(n);
		return e == 0 ? edgeFrom(n) : e;
	}
	bool isAdjacentTo(const Node* n) const
	{
		return hasEdgeTo(n) || hasEdgeFrom(n);
	}
	virtual edge_iterator_range outEdges() const = 0;
	virtual edge_iterator_range inEdges() const = 0;
	virtual OutNeighborIteratorRange outNeighbors() const = 0;
	virtual InNeighborIteratorRange inNeighbors() const = 0;

	template<class RandomNumGen>
	Node* randomOutNeighbor(RandomNumGen& rnd)
	{
		const degree_size_t deg = outDegree();
		if (deg == 0)
			return 0;
		const int num = rnd.IntFromTo(0, deg - 1);
		OutNeighborIteratorRange iters = outNeighbors();
		OutNeighborIterator it = iters.first;
		std::advance(it, num);
		if (it != iters.second) // should always give true
			return it.operator->();
		else
			return 0;
	}

	template<class RandomNumGen>
	const Node* randomOutNeighbor(RandomNumGen& rnd) const
	{
		const degree_size_t deg = outDegree();
		if (deg == 0)
			return 0;
		const int num = rnd.IntFromTo(0, deg - 1);
		ConstOutNeighborIteratorRange iters = outNeighbors();
		ConstOutNeighborIterator it = iters.first;
		std::advance(it, num);
		if (it != iters.second) // should always give true
			return it.operator->();
		else
			return 0;
	}

	template<class RandomNumGen>
	Node* randomInNeighbor(RandomNumGen& rnd)
	{
		const degree_size_t deg = inDegree();
		if (deg == 0)
			return 0;
		const int num = rnd.IntFromTo(0, deg - 1);
		InNeighborIteratorRange iters = inNeighbors();
		InNeighborIterator it = iters.first;
		std::advance(it, num);
		if (it != iters.second) // should always give true
			return it.operator->();
		else
			return 0;
	}

	template<class RandomNumGen>
	const Node* randomInNeighbor(RandomNumGen& rnd) const
	{
		const degree_size_t deg = inDegree();
		if (deg == 0)
			return 0;
		const int num = rnd.IntFromTo(0, deg - 1);
		ConstInNeighborIteratorRange iters = inNeighbors();
		ConstInNeighborIterator it = iters.first;
		std::advance(it, num);
		if (it != iters.second) // should always give true
			return it.operator->();
		else
			return 0;
	}

protected:
	virtual void registerEdge(Edge* e) = 0;
	virtual void unregisterEdge(Edge* e) = 0;
	void setId(node_id_t id)
	{
		id_ = id;
	}

private:
	node_id_t id_;
};

}
#endif /*NODE_H_*/