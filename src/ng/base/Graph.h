/**
 * @file DirectedGraph.h
 * @date 12.08.2009
 * @author gerd
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "types.h"
#include "../../repo/CRepository.h"
#include "GraphListener.h"
#include <boost/noncopyable.hpp>
#include <list>
#include <utility>
#include <memory>
#include <iterator>

namespace largenet
{

class Node;
class Edge;
class ElementFactory;

class Graph: public boost::noncopyable
{
private:
	typedef repo::CRepository<Node*> NodeContainer; // FIXME these shoud be proper pointer containers...
	typedef repo::CRepository<Edge*> EdgeContainer;
	typedef std::list<GraphListener*> ListenerContainer; // use boost::ptr_list if taking ownership seems better

public:
	class NodeIterator: public std::iterator<std::iterator_traits<
			NodeContainer::iterator>::iterator_category, node_id_t>
	{
	public:
		explicit NodeIterator(const NodeContainer::iterator& i) :
			it_(i)
		{
		}
		NodeIterator()
		{
		}
		NodeIterator(const NodeIterator& other) :
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
		NodeIterator& operator=(const NodeIterator& i)
		{
			if (&i != this)
				it_ = i.it_;
			return *this;
		}
		bool operator==(const NodeIterator& i)
		{
			return it_ == i.it_;
		}
		bool operator!=(const NodeIterator& i)
		{
			return !this->operator==(i);
		}
		NodeIterator& operator++()
		{
			++it_;
			return *this;
		}
		NodeIterator operator++(int)
		{
			NodeIterator temp(*this);
			++(*this);
			return temp;
		}
	private:
		NodeContainer::iterator it_;
	};

	class EdgeIterator: public std::iterator<std::iterator_traits<
			EdgeContainer::iterator>::iterator_category, edge_id_t>
	{
	public:
		explicit EdgeIterator(const EdgeContainer::iterator& i) :
			it_(i)
		{
		}
		EdgeIterator()
		{
		}
		EdgeIterator(const EdgeIterator& other) :
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
		EdgeIterator& operator=(const EdgeIterator& i)
		{
			if (&i != this)
				it_ = i.it_;
			return *this;
		}
		bool operator==(const EdgeIterator& i)
		{
			return it_ == i.it_;
		}
		bool operator!=(const EdgeIterator& i)
		{
			return !this->operator==(i);
		}
		EdgeIterator& operator++()
		{
			++it_;
			return *this;
		}
		EdgeIterator operator++(int)
		{
			EdgeIterator temp(*this);
			++(*this);
			return temp;
		}
	private:
		EdgeContainer::iterator it_;
	};

	typedef std::pair<NodeIterator, NodeIterator> NodeIteratorRange;
	typedef std::pair<EdgeIterator, EdgeIterator> EdgeIteratorRange;

	Graph(node_state_t nodeStates, edge_state_t edgeStates);
	virtual ~Graph();
	void setElementFactory(std::auto_ptr<ElementFactory> elf);
	void addGraphListener(GraphListener* l);
	void removeGraphListener(GraphListener* l);
	void clear();

	node_size_t numberOfNodes() const;
	node_size_t numberOfNodes(node_state_t s) const;
	edge_size_t numberOfEdges() const;
	edge_size_t numberOfEdges(edge_state_t s) const;
	bool isDirected() const;

	node_id_t addNode();
	node_id_t addNode(node_state_t s);
	edge_id_t addEdge(node_id_t source, node_id_t target);
	void removeNode(node_id_t n);
	void removeEdge(edge_id_t e);
	Node* node(node_id_t n);
	const Node* node(node_id_t n) const;
	Edge* edge(edge_id_t e);
	const Edge* edge(edge_id_t e) const;

	void setNodeState(node_id_t n, node_state_t s);
	void setEdgeState(edge_id_t e, edge_state_t s);
	node_state_t nodeState(node_id_t n) const;
	edge_state_t edgeState(edge_id_t e) const;

	NodeIteratorRange nodes();
	EdgeIteratorRange edges();

	bool isEdge(node_id_t source, node_id_t target) const;

private:
	void afterNodeAdd(node_id_t n);
	void afterEdgeAdd(edge_id_t e);
	void beforeNodeRemove(node_id_t n);
	void beforeEdgeRemove(edge_id_t e);
	void beforeGraphClear();
	void afterNodeStateChange(node_id_t n, node_state_t oldState,
			node_state_t newState);
	void afterEdgeStateChange(edge_id_t e, edge_state_t oldState,
			edge_state_t newState);

	std::auto_ptr<ElementFactory> elf_;
	NodeContainer nodes_;
	EdgeContainer edges_;
	ListenerContainer listeners_;
};

inline node_size_t Graph::numberOfNodes() const
{
	return nodes_.size();
}

inline node_size_t Graph::numberOfNodes(const node_state_t s) const
{
	return nodes_.count(s);
}

inline edge_size_t Graph::numberOfEdges() const
{
	return edges_.size();
}

inline edge_size_t Graph::numberOfEdges(const edge_state_t s) const
{
	return edges_.count(s);
}

inline Node* Graph::node(const node_id_t n)
{
	if (nodes_.valid(n))
		return nodes_[n];
	else
		return 0;
}

inline const Node* Graph::node(const node_id_t n) const
{
	if (nodes_.valid(n))
		return nodes_[n];
	else
		return 0;
}

inline Edge* Graph::edge(const edge_id_t e)
{
	if (edges_.valid(e))
		return edges_[e];
	else
		return 0;
}

inline const Edge* Graph::edge(const edge_id_t e) const
{
	if (edges_.valid(e))
		return edges_[e];
	else
		return 0;
}

inline node_state_t Graph::nodeState(const node_id_t n) const
{
	return nodes_.category(n);
}

inline edge_state_t Graph::edgeState(const edge_id_t e) const
{
	return edges_.category(e);
}

inline void Graph::setElementFactory(std::auto_ptr<ElementFactory> elf)
{
	if (elf_.get() != elf.get())
	{
		elf_.reset(); // destroy and delete old factory
		elf_ = elf;
	}
}

inline void Graph::addGraphListener(GraphListener* l)
{
	listeners_.push_back(l);
}

inline void Graph::removeGraphListener(GraphListener* l)
{
	listeners_.remove(l);
}

inline void Graph::setNodeState(const node_id_t n, const node_state_t s)
{
	const node_state_t old = nodeState(n);
	if (s == old)
		return;
	nodes_.setCategory(n, s);
	afterNodeStateChange(n, old, s);
}

inline void Graph::setEdgeState(const edge_id_t e, const edge_state_t s)
{
	const edge_state_t old = edgeState(e);
	if (s == old)
		return;
	edges_.setCategory(e, s);
	afterEdgeStateChange(e, old, s);
}

}

#endif /* GRAPH_H_ */
