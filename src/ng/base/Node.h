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
#include "Edge.h"
#include <set>
#include <utility>
#include <boost/noncopyable.hpp>

namespace largenet
{

//class Edge;

class Node: public boost::noncopyable
{
	friend class Edge;
public:
	typedef std::set<Edge*> edge_set;
	typedef edge_set::iterator edge_iterator;
	typedef edge_set::const_iterator const_edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;

	class OutNeighborIterator : public std::iterator<std::iterator_traits<edge_iterator>::iterator_category, node_id_t>
	{
	public:
		OutNeighborIterator() : node_(0) {}
		OutNeighborIterator(const Node* n, edge_iterator it) : node_(n), it_(it) {}
		OutNeighborIterator(const OutNeighborIterator& other) : node_(other.node_), it_(other.it_) {}
		OutNeighborIterator& operator=(const OutNeighborIterator& other) { if (&other != this) { node_ = other.node_; it_ = other.it_; } return *this; }
		bool operator==(const OutNeighborIterator& other) { return (other.node_ == node_) && (other.it_ == it_); }
		bool operator!=(const OutNeighborIterator& other) { return !operator==(other); }
		node_id_t operator*() { return (*it_)->target()->id(); }
		Node* operator->() { return (*it_)->target(); }
		OutNeighborIterator& operator++() { ++it_; return *this; }
		OutNeighborIterator operator++(int) { OutNeighborIterator temp(*this); ++(*this); return temp; }

	private:
		const Node* node_;
		edge_iterator it_;
	};

	class InNeighborIterator : public std::iterator<std::iterator_traits<edge_iterator>::iterator_category, node_id_t>
	{
	public:
		InNeighborIterator() : node_(0) {}
		InNeighborIterator(const Node* n, edge_iterator it) : node_(n), it_(it) {}
		InNeighborIterator(const InNeighborIterator& other) : node_(other.node_), it_(other.it_) {}
		InNeighborIterator& operator=(const InNeighborIterator& other) { if (&other != this) { node_ = other.node_; it_ = other.it_; } return *this; }
		bool operator==(const InNeighborIterator& other) { return (other.node_ == node_) && (other.it_ == it_); }
		bool operator!=(const InNeighborIterator& other) { return !operator==(other); }
		node_id_t operator*() { return (*it_)->source()->id(); }
		Node* operator->() { return (*it_)->source(); }
		InNeighborIterator& operator++() { ++it_; return *this; }
		InNeighborIterator operator++(int) { InNeighborIterator temp(*this); ++(*this); return temp; }

	private:
		const Node* node_;
		edge_iterator it_;
	};

	typedef std::pair<OutNeighborIterator, OutNeighborIterator> OutNeighborIteratorRange;
	typedef std::pair<InNeighborIterator, InNeighborIterator> InNeighborIteratorRange;

	Node(node_id_t id) : id_(id) {}
	virtual ~Node() {}
	node_id_t id() const { return id_; }
	virtual degree_size_t outDegree() const = 0;
	virtual degree_size_t inDegree() const = 0;
	degree_size_t degree() const { return outDegree() + inDegree(); }
	virtual bool hasInEdge(Edge* e) const = 0;
	virtual bool hasOutEdge(Edge* e) const = 0;
	bool hasEdge(Edge* e) const { return hasOutEdge(e) || hasInEdge(e); }
	virtual bool hasEdgeTo(const Node* n) const = 0;
	virtual bool hasEdgeFrom(const Node* n) const = 0;
	virtual Edge* edgeTo(const Node* n) const = 0;
	virtual Edge* edgeFrom(const Node* n) const = 0;
	Edge* edgeToAdjacentNode(const Node* n) const { Edge* e = edgeTo(n); return e == 0 ? edgeFrom(n) : e; }
	bool isAdjacentTo(const Node* n) const { return hasEdgeTo(n) || hasEdgeFrom(n); }
	virtual edge_iterator_range outEdges() const = 0;
	virtual edge_iterator_range inEdges() const = 0;
	virtual OutNeighborIteratorRange outNeighbors() const = 0;
	virtual InNeighborIteratorRange inNeighbors() const = 0;

protected:
	virtual void registerEdge(Edge* e) = 0;
	virtual void unregisterEdge(Edge* e) = 0;
	void setId(node_id_t id) { id_ = id; }

private:
	node_id_t id_;
};

}
#endif /*NODE_H_*/
