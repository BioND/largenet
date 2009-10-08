/**
 * @file graph_iterators.h
 * @date 08.10.2009
 * @author gerd
 */

#ifndef GRAPH_ITERATORS_H_
#define GRAPH_ITERATORS_H_

#include "types.h"
#include <iterator>

namespace largenet
{

class Node;
class Edge;

namespace iterators
{

template<class NodeContainer>
class GraphNodeIterator: public std::iterator<typename std::iterator_traits<
		typename NodeContainer::iterator>::iterator_category, node_id_t>
{
public:
	explicit GraphNodeIterator(const typename NodeContainer::iterator& i) :
		it_(i)
	{
	}
	GraphNodeIterator()
	{
	}
	GraphNodeIterator(const GraphNodeIterator& other) :
		it_(other.it_)
	{
	}
	node_id_t operator*()
	{
		return it_.id();
	}
	Node* operator->()
	{
		return *it_;
	}
	GraphNodeIterator& operator=(const GraphNodeIterator& i)
	{
		if (&i != this)
			it_ = i.it_;
		return *this;
	}
	bool operator==(const GraphNodeIterator& i)
	{
		return it_ == i.it_;
	}
	bool operator!=(const GraphNodeIterator& i)
	{
		return !this->operator==(i);
	}
	GraphNodeIterator& operator++()
	{
		++it_;
		return *this;
	}
	GraphNodeIterator operator++(int)
	{
		GraphNodeIterator temp(*this);
		++(*this);
		return temp;
	}
private:
	typename NodeContainer::iterator it_;
};

template<class EdgeContainer>
class GraphEdgeIterator: public std::iterator<typename std::iterator_traits<
		typename EdgeContainer::iterator>::iterator_category, edge_id_t>
{
public:
	explicit GraphEdgeIterator(const typename EdgeContainer::iterator& i) :
		it_(i)
	{
	}
	GraphEdgeIterator()
	{
	}
	GraphEdgeIterator(const GraphEdgeIterator& other) :
		it_(other.it_)
	{
	}
	edge_id_t operator*()
	{
		return it_.id();
	}
	Edge* operator->()
	{
		return *it_;
	}
	GraphEdgeIterator& operator=(const GraphEdgeIterator& i)
	{
		if (&i != this)
			it_ = i.it_;
		return *this;
	}
	bool operator==(const GraphEdgeIterator& i)
	{
		return it_ == i.it_;
	}
	bool operator!=(const GraphEdgeIterator& i)
	{
		return !this->operator==(i);
	}
	GraphEdgeIterator& operator++()
	{
		++it_;
		return *this;
	}
	GraphEdgeIterator operator++(int)
	{
		GraphEdgeIterator temp(*this);
		++(*this);
		return temp;
	}
private:
	typename EdgeContainer::iterator it_;
};
}
}

#endif /* GRAPH_ITERATORS_H_ */
