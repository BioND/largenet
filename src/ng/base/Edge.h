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

class Edge: public boost::noncopyable
{
public:
	Edge();
	Edge(edge_id_t id, Node* source, Node* target);
	~Edge();
	Node* source() const { return source_; }
	Node* target() const { return target_; }
	Node* opposite(const Node* from) const;
	bool isLoop() const;

private:
	void connect();
	void disconnect();
	edge_id_t id_;
	Node* source_;
	Node* target_;
};

inline bool Edge::isLoop() const
{
	return source_ == target_;
}

}

#endif /* EDGE_H_ */
