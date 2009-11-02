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

class Node;

/**
 * A directed edge.
 */
class Edge: public boost::noncopyable
{
public:
	Edge();
	Edge(edge_id_t id, Node* source, Node* target);
	virtual ~Edge();
	virtual bool isDirected() const { return true; }
	edge_id_t id() const { return id_; }
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
	UndirectedEdge() : Edge() {}
	UndirectedEdge(edge_id_t id, Node* source, Node* target);
	virtual ~UndirectedEdge() {}
	virtual bool isDirected() const { return false; }
	virtual bool operator==(const Edge& e) const;
};

inline bool UndirectedEdge::operator==(const Edge& e) const
{
	return ((source() == e.source()) && (target() == e.target())) || ((source() == e.target()) && (target() == e.source()));
}

inline bool is_directed(const Edge& e)
{
	return e.isDirected();
}

}

#endif /* EDGE_H_ */
