/**
 * @file Edge.h
 * @date 12.08.2009
 * @author gerd
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "types.h"
#include <boost/noncopyable.hpp>

namespace largenet
{

struct directed_edge_tag {};
struct undirected_edge_tag {};

class Node;

/**
 * A directed edge.
 */
class Edge: public boost::noncopyable
{
public:
	typedef directed_edge_tag directed_category;
	Edge();
	Edge(edge_id_t id, Node* source, Node* target);
	virtual ~Edge();
	Node* source() const { return source_; }
	Node* target() const { return target_; }
	Node* opposite(const Node* from) const;
	bool isLoop() const { return source_ == target_; }
	virtual bool operator==(const Edge& e) const { return (source_ == e.source_) && (target_ == e.target_); }

private:
	void connect();
	void disconnect();
	edge_id_t id_;
	Node* source_;
	Node* target_;
};

typedef Edge DirectedEdge;

class UndirectedEdge: public Edge
{
public:
	typedef undirected_edge_tag directed_category;
	UndirectedEdge() : Edge() {}
	UndirectedEdge(edge_id_t id, Node* source, Node* target);
	virtual ~UndirectedEdge() {}
	virtual bool operator==(const Edge& e) const;
};

inline bool UndirectedEdge::operator==(const Edge& e) const
{
	return ((source() == e.source()) && (target() == e.target())) || ((source() == e.target()) && (target() == e.source()));
}

namespace detail
{
	inline bool is_directed(directed_edge_tag) { return true; }
	inline bool is_directed(undirected_edge_tag) { return false; }
}

template<class E>
inline bool is_directed(const E&)
{
	typedef typename E::directed_category Cat;
	return detail::is_directed(Cat());
}

}

#endif /* EDGE_H_ */
