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

	Node(node_id_t id) : id_(id) {}
	virtual ~Node() {}
	node_id_t id() const { return id_; }
	virtual degree_size_t outDegree() const = 0;
	virtual degree_size_t inDegree() const = 0;
	degree_size_t degree() const { return outDegree() + inDegree(); }
	virtual bool hasInEdge(Edge* e) const = 0;
	virtual bool hasOutEdge(Edge* e) const = 0;
	bool hasEdge(Edge* e) const { return hasOutEdge(e) || hasInEdge(e); }
	virtual bool hasEdgeTo(Node* n) const = 0;
	virtual bool hasEdgeFrom(Node* n) const = 0;
	bool isAdjacentTo(Node* n) const { return hasEdgeTo(n) || hasEdgeFrom(n); }
	virtual edge_iterator_range outEdges() const = 0;
	virtual edge_iterator_range inEdges() const = 0;

protected:
	virtual void registerEdge(Edge* e) = 0;
	virtual void unregisterEdge(Edge* e) = 0;
	void setId(node_id_t id) { id_ = id; }

private:
	node_id_t id_;
};

}
#endif /*NODE_H_*/
