/**
 * @file DirectedGraph.h
 * @date 12.08.2009
 * @author gerd
 */

#ifndef DIRECTEDGRAPH_H_
#define DIRECTEDGRAPH_H_

#include "traits.h"
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <utility>
#include <memory>
#include <iterator>

namespace lnet
{

template<class G> class Node;
template<class G> class Edge;
template<class G> class ElementFactory;

class DirectedGraph: public boost::noncopyable
{
public:
	typedef unsigned long int node_descriptor;
	typedef unsigned long int edge_descriptor;
	typedef node_descriptor node_id_t;
	typedef edge_descriptor edge_id_t;
	typedef directed_tag directed_category;
	typedef node_descriptor degree_size_type;
	typedef node_descriptor vertices_size_type;
	typedef edge_descriptor edges_size_type;
	typedef Node<DirectedGraph> NodeType;
	typedef Edge<DirectedGraph> EdgeType;
private:
	typedef boost::ptr_map<node_id_t, NodeType> NodeContainer;
	typedef boost::ptr_map<edge_id_t, EdgeType> EdgeContainer;

public:
	typedef NodeContainer::size_type node_count_t;
	typedef NodeContainer::size_type edge_count_t;

	typedef NodeContainer::iterator NodeIterator;

	//	class NodeIterator : public std::iterator<std::iterator_traits<NodeContainer::iterator>::iterator_category, node_id_t>
	//	{
	//	public:
	//		explicit NodeIterator(const NodeContainer::iterator& i) : it_(i) {}
	//		NodeIterator() {}
	//		NodeIterator(const NodeIterator& other) : it_(other.it_) {}
	//		node_id_t operator*() { return it_->first; }
	//		Node* operator->() { return it_->second; }
	//		NodeIterator& operator=(const NodeIterator& i) { if (&i != this) it_ = i.it_;  return *this; }
	//		bool operator==(const NodeIterator& i) { return it_ == i.it_; }
	//		bool operator!=(const NodeIterator& i) { return !this->operator==(i); }
	//		NodeIterator& operator++() { ++it_; return *this; }
	//		NodeIterator operator++(int) { NodeIterator temp(*this); ++(*this); return temp; }
	//	private:
	//		NodeContainer::iterator it_;
	//	};

	typedef EdgeContainer::iterator EdgeIterator;
	//	class EdgeIterator : public std::iterator<std::iterator_traits<EdgeContainer::iterator>::iterator_category, edge_id_t>
	//	{
	//	public:
	//		explicit EdgeIterator(const EdgeContainer::iterator& i) : it_(i) {}
	//		EdgeIterator() {}
	//		EdgeIterator(const EdgeIterator& other) : it_(other.it_) {}
	//		edge_id_t operator*() { return it_->first; }
	//		EdgeProxy<edge_id_t> operator->() { return EdgeProxy<edge_id_t>(it_->second, it_->first); }
	//		EdgeIterator& operator=(const EdgeIterator& i) { if (&i != this) it_ = i.it_; return *this; }
	//		bool operator==(const EdgeIterator& i) { return it_ == i.it_; }
	//		bool operator!=(const EdgeIterator& i) { return !this->operator==(i); }
	//		EdgeIterator& operator++() { ++it_; return *this; }
	//		EdgeIterator operator++(int) { EdgeIterator temp(*this); ++(*this); return temp; }
	//	private:
	//		EdgeContainer::iterator it_;
	//	};

	typedef std::pair<NodeIterator, NodeIterator> NodeIteratorRange;
	typedef std::pair<EdgeIterator, EdgeIterator> EdgeIteratorRange;

	DirectedGraph();
	virtual ~DirectedGraph();
	void setElementFactory(std::auto_ptr<ElementFactory<DirectedGraph> > elf);
	void clear();
	node_count_t numberOfNodes() const;
	edge_count_t numberOfEdges() const;
	node_id_t addNode();
	edge_id_t addEdge(node_id_t source, node_id_t target);
	void removeNode(node_id_t n);
	void removeEdge(edge_id_t e);
	NodeType* node(node_id_t n);
	EdgeType* edge(edge_id_t e);

	NodeIteratorRange nodes();
	EdgeIteratorRange edges();

private:
	std::auto_ptr<ElementFactory<DirectedGraph> > elf_;
	NodeContainer nodes_;
	EdgeContainer edges_;
};

inline DirectedGraph::node_count_t DirectedGraph::numberOfNodes() const
{
	return nodes_.size();
}

inline DirectedGraph::edge_count_t DirectedGraph::numberOfEdges() const
{
	return edges_.size();
}

inline DirectedGraph::NodeType* DirectedGraph::node(const node_id_t n)
{
	NodeContainer::iterator pos(nodes_.find(n));
	if (pos != nodes_.end())
		return pos->second;
	else
		return 0;
}

inline DirectedGraph::EdgeType* DirectedGraph::edge(const edge_id_t e)
{
	EdgeContainer::iterator pos(edges_.find(e));
	if (pos != edges_.end())
		return pos->second;
	else
		return 0;
}

inline
void DirectedGraph::setElementFactory(std::auto_ptr<ElementFactory<
		DirectedGraph> > elf)
{
	if (elf_.get() != elf.get())
	{
		elf_.reset();
		elf_ = elf;
	}
}

}

#endif /* DIRECTEDGRAPH_H_ */
