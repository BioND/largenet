/**
 * @file Node.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Node class.
 */
#ifndef NODE_H_
#define NODE_H_

#include "traits.h"
#include <set>
#include <utility>

namespace lnet
{

struct directedS
{
};
struct undirectedS
{
};


namespace detail
{

template<typename edge_descriptor>
class UndirectedNodeP
{
public:
	typedef std::set<edge_descriptor> edge_set;
	typedef typename edge_set::size_type degree_size_type;
	typedef typename edge_set::iterator edge_iterator;
	typedef typename edge_set::const_iterator const_edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;
	typedef std::pair<const_edge_iterator, const_edge_iterator>
			const_edge_iterator_range;
protected:
	~UndirectedNodeP()
	{
	}
	edge_set outEdges_;

public:
	UndirectedNodeP()
	{
	}
	degree_size_type degree() const
	{
		return outDegree();
	}
	degree_size_type outDegree() const
	{
		return outEdges_.size();
	}
	void addOutEdge(edge_descriptor e)
	{
		outEdges_.insert(e);
	}
	void addEdge(edge_descriptor e)
	{
		addOutEdge(e);
	}
	void removeEdge(edge_descriptor e)
	{
		edge_iterator it = outEdges_.find(e);
		if (it != outEdges_.end())
			outEdges_.erase(it);
	}
	bool hasOutEdge(edge_descriptor e) const
	{
		return outEdges_.find(e) != outEdges_.end();
	}
	bool hasEdge(edge_descriptor e) const
	{
		return hasOutEdge(e);
	}

	edge_iterator_range outEdges() const
	{
		return std::make_pair(outEdges_.begin(), outEdges_.end());
	}
	edge_iterator_range edges() const
	{
		return outEdges();
	}
};

template<typename edge_descriptor>
class DirectedNodeP
{
public:
	typedef std::set<edge_descriptor> edge_set;
	typedef typename edge_set::size_type degree_size_type;
	typedef typename edge_set::iterator edge_iterator;
	typedef typename edge_set::const_iterator const_edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;
	typedef std::pair<const_edge_iterator, const_edge_iterator>
			const_edge_iterator_range;
protected:
	~DirectedNodeP()
	{
	}
	edge_set outEdges_, inEdges_;

public:
	DirectedNodeP()
	{
	}
	degree_size_type degree() const
	{
		return outDegree() + inDegree();
	}
	degree_size_type outDegree() const
	{
		return outEdges_.size();
	}
	degree_size_type inDegree() const
	{
		return inEdges_.size();
	}
	void addOutEdge(edge_descriptor e)
	{
		outEdges_.insert(e);
	}
	void addInEdge(edge_descriptor e)
	{
		inEdges_.insert(e);
	}
	void removeEdge(edge_descriptor e)
	{
		edge_iterator it = inEdges_.find(e);
		if (it != inEdges_.end())
			inEdges_.erase(it);
		it = outEdges_.find(e);
		if (it != outEdges_.end())
			outEdges_.erase(it);
	}
	bool hasOutEdge(edge_descriptor e) const
	{
		return outEdges_.find(e) != outEdges_.end();
	}
	bool hasInEdge(edge_descriptor e) const
	{
		return inEdges_.find(e) != inEdges_.end();
	}
	bool hasEdge(edge_descriptor e) const
	{
		return hasInEdge(e) || hasOutEdge(e);
	}

	edge_iterator_range outEdges() const
	{
		return std::make_pair(outEdges_.begin(), outEdges_.end());
	}
	edge_iterator_range inEdges() const
	{
		return std::make_pair(inEdges_.begin(), inEdges_.end());
	}
};

template<class dirSelector, typename edge_descriptor> struct directed_policy_tag;

template<typename edge_descriptor>
struct directed_policy_tag<directedS, edge_descriptor>
{
	typedef DirectedNodeP<edge_descriptor> policy;
};

template<typename edge_descriptor>
struct directed_policy_tag<undirectedS, edge_descriptor>
{
	typedef UndirectedNodeP<edge_descriptor> policy;
};

}

template<class dirSelector, typename edge_descriptor>
class Node: public detail::directed_policy_tag<dirSelector, edge_descriptor>::policy
{
public:
	Node() : detail::directed_policy_tag<dirSelector, edge_descriptor>::policy()
	{
	}
};

}
#endif /*NODE_H_*/
