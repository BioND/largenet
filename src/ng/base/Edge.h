/**
 * @file Edge.h
 * @date 12.08.2009
 * @author gerd
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "traits.h"
#include "Node.h"
#include <boost/noncopyable.hpp>

namespace lnet
{

//template<class G> class Node;

template<class G>
class Edge: public boost::noncopyable
{
public:
	typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
	typedef typename graph_traits<G>::node_descriptor node_descriptor;
	typedef Node<G>* NodePtr;

	Edge();
	Edge(edge_descriptor id, NodePtr source, NodePtr target);
	~Edge();
	NodePtr source() const { return source_; }
	NodePtr target() const { return target_; }
	NodePtr opposite(const NodePtr from) const;
	bool isLoop() const;

private:
	void connect();
	void disconnect();
	edge_descriptor id_;
	NodePtr source_;
	NodePtr target_;
};

template<class G>
Edge<G>::Edge() :
	id_(0), source_(0), target_(0)
{
}

template<class G>
Edge<G>::Edge(const edge_descriptor id, NodePtr source, NodePtr target) :
	id_(id), source_(source), target_(target)
{
	connect();
}

template<class G>
Edge<G>::~Edge()
{
	disconnect();
}

template<class G>
typename Edge<G>::NodePtr Edge<G>::opposite(const NodePtr from) const
{
	if (from == source_)
		return target_;
	else if (from == target_)
		return source_;
	else
		return 0;
}

template<class G>
bool Edge<G>::isLoop() const
{
	return source_ == target_;
}

template<class G>
void Edge<G>::connect()
{
	if ((source_ == 0) || (target_ == 0)) return;
	source_->registerEdge(this);
	target_->registerEdge(this);
}

template<class G>
void Edge<G>::disconnect()
{
	if ((source_ == 0) || (target_ == 0)) return;
	source_->unregisterEdge(this);
	target_->unregisterEdge(this);
}

}

#endif /* EDGE_H_ */
