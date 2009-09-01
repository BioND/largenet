/**
 * @file DirectedGraph.h
 * @date 12.08.2009
 * @author gerd
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "types.h"
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <utility>
#include <memory>

namespace largenet
{

class Node;
class Edge;
class ElementFactory;

class Graph: public boost::noncopyable
{
private:
	typedef boost::ptr_map<node_id_t, Node> NodeContainer;
	typedef boost::ptr_map<edge_id_t, Edge> EdgeContainer;

public:
	class NodeIterator : public std::iterator<std::iterator_traits<NodeContainer::iterator>::iterator_category, node_id_t>
	{
	public:
		explicit NodeIterator(const NodeContainer::iterator& i) : it_(i) {}
		NodeIterator() {}
		NodeIterator(const NodeIterator& other) : it_(other.it_) {}
		node_id_t operator*() { return it_->first; }
		Node* operator->() { return it_->second; }
		NodeIterator& operator=(const NodeIterator& i) { if (&i != this) it_ = i.it_;  return *this; }
		bool operator==(const NodeIterator& i) { return it_ == i.it_; }
		bool operator!=(const NodeIterator& i) { return !this->operator==(i); }
		NodeIterator& operator++() { ++it_; return *this; }
		NodeIterator operator++(int) { NodeIterator temp(*this); ++(*this); return temp; }
	private:
		NodeContainer::iterator it_;
	};

	class EdgeIterator : public std::iterator<std::iterator_traits<EdgeContainer::iterator>::iterator_category, edge_id_t>
	{
	public:
		explicit EdgeIterator(const EdgeContainer::iterator& i) : it_(i) {}
		EdgeIterator() {}
		EdgeIterator(const EdgeIterator& other) : it_(other.it_) {}
		edge_id_t operator*() { return it_->first; }
		Edge* operator->() { return it_->second; }
		EdgeIterator& operator=(const EdgeIterator& i) { if (&i != this) it_ = i.it_; return *this; }
		bool operator==(const EdgeIterator& i) { return it_ == i.it_; }
		bool operator!=(const EdgeIterator& i) { return !this->operator==(i); }
		EdgeIterator& operator++() { ++it_; return *this; }
		EdgeIterator operator++(int) { EdgeIterator temp(*this); ++(*this); return temp; }
	private:
		EdgeContainer::iterator it_;
	};

	typedef std::pair<NodeIterator, NodeIterator> NodeIteratorRange;
	typedef std::pair<EdgeIterator, EdgeIterator> EdgeIteratorRange;

	Graph();
	virtual ~Graph();
	void setElementFactory(std::auto_ptr<ElementFactory> elf);
	void clear();
	node_size_t numberOfNodes() const;
	edge_size_t numberOfEdges() const;
	node_id_t addNode();
	edge_id_t addEdge(node_id_t source, node_id_t target);
	void removeNode(node_id_t n);
	void removeEdge(edge_id_t e);
	Node* node(node_id_t n);
	Edge* edge(edge_id_t e);

	NodeIteratorRange nodes();
	EdgeIteratorRange edges();

private:
	std::auto_ptr<ElementFactory> elf_;
	NodeContainer nodes_;
	EdgeContainer edges_;
};

inline node_size_t Graph::numberOfNodes() const
{
	return nodes_.size();
}

inline edge_size_t Graph::numberOfEdges() const
{
	return edges_.size();
}

inline Node* Graph::node(const node_id_t n)
{
	NodeContainer::iterator pos(nodes_.find(n));
	if (pos != nodes_.end())
		return pos->second;
	else
		return 0;
}

inline Edge* Graph::edge(const edge_id_t e)
{
	EdgeContainer::iterator pos(edges_.find(e));
	if (pos != edges_.end())
		return pos->second;
	else
		return 0;
}

inline void Graph::setElementFactory(std::auto_ptr<ElementFactory> elf)
{
	if (elf_.get() != elf.get())
	{
		elf_.reset();
		elf_ = elf;
	}
}

}

#endif /* GRAPH_H_ */
