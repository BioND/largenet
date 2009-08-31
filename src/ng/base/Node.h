/**
 * @file Node.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Node class.
 */
#ifndef NODE_H_
#define NODE_H_

#include "traits.h"
#include "exceptions.h"
#include <set>
#include <utility>
#include <boost/noncopyable.hpp>

namespace lnet
{

template<class G> class Edge;

template<class G>
class Node: public boost::noncopyable
{
	friend class Edge<G>;
public:
	typedef typename graph_traits<G>::node_descriptor node_descriptor;
	typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
	typedef std::set<Edge<G>* > edge_set;
	typedef typename edge_set::size_type degree_size_type;
	typedef typename edge_set::iterator edge_iterator;
	typedef typename edge_set::const_iterator const_edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;

	Node(node_descriptor id) : id_(id) {}
	virtual ~Node() {}
	node_descriptor id() const { return id_; }
	virtual degree_size_type outDegree() const = 0;
	virtual degree_size_type inDegree() const = 0;
	degree_size_type degree() const { return outDegree() + inDegree(); }
	virtual bool hasInEdge(Edge<G>* e) const = 0;
	virtual bool hasOutEdge(Edge<G>* e) const = 0;
	bool hasEdge(Edge<G>* e) const { return hasOutEdge(e) || hasInEdge(e); }
	virtual bool hasEdgeTo(Node* n) const = 0;
	virtual bool hasEdgeFrom(Node* n) const = 0;
	bool isAdjacentTo(Node* n) const { return hasEdgeTo(n) || hasEdgeFrom(n); }
	virtual edge_iterator_range outEdges() const = 0;
	virtual edge_iterator_range inEdges() const = 0;

protected:
	virtual void registerEdge(Edge<G>* e) = 0;
	virtual void unregisterEdge(Edge<G>* e) = 0;
	void setId(node_descriptor id) { id_ = id; }

private:
	node_descriptor id_;
};

template<class G>
class SingleNode: public Node<G>
{
public:
	typedef Node<G> base_type;
	SingleNode(typename base_type::node_descriptor id) : Node<G>(id) {}
	virtual ~SingleNode() {}
	virtual typename base_type::degree_size_type outDegree() const { return outEdges_.size(); }
	virtual typename base_type::degree_size_type inDegree() const { return inEdges_.size(); }
	virtual bool hasInEdge(Edge<G>* e) const { return inEdges_.find(e) != inEdges_.end(); }
	virtual bool hasOutEdge(Edge<G>* e) const { return outEdges_.find(e) != outEdges_.end(); }
	virtual bool hasEdgeTo(Node<G>* n) const;
	virtual bool hasEdgeFrom(Node<G>* n) const;
	virtual typename base_type::edge_iterator_range outEdges() const { return std::make_pair(outEdges_.begin(), outEdges_.end()); }
	virtual typename base_type::edge_iterator_range inEdges() const { return std::make_pair(inEdges_.begin(), inEdges_.end()); }

protected:
	virtual void registerEdge(Edge<G>* e);
	virtual void unregisterEdge(Edge<G>* e);

private:
	typename base_type::edge_set outEdges_, inEdges_;
};

template<class G>
bool SingleNode<G>::hasEdgeTo(Node<G>* n) const
{
	for (typename base_type::edge_set::const_iterator it = outEdges_.begin(); it != outEdges_.end(); ++it)
	{
		if ((*it)->target() == n)
			return true;
	}
	return false;
}

template<class G>
bool SingleNode<G>::hasEdgeFrom(Node<G>* n) const
{
	for (typename base_type::edge_set::const_iterator it = inEdges_.begin(); it != inEdges_.end(); ++it)
	{
		if ((*it)->source() == n)
			return true;
	}
	return false;
}

template<class G>
void SingleNode<G>::registerEdge(Edge<G>* e)
{
	if (hasEdge(e))
		return;
	if (e->source() == this)
	{
		if (hasEdgeTo(e->target()))
			throw SingletonException("Edge already exists.");
		outEdges_.insert(e);
	} else if (e->target() == this)
	{
		if (hasEdgeFrom(e->source()))
			throw SingletonException("Edge already exists.");
		inEdges_.insert(e);
	}
}

template<class G>
void SingleNode<G>::unregisterEdge(Edge<G>* e)
{
	if (hasInEdge(e))
		inEdges_.erase(e);
	else if (hasOutEdge(e))
		outEdges_.erase(e);
}

}
#endif /*NODE_H_*/
