/**
 * @file EdgeProxy.h
 * @date 13.08.2009
 * @author gerd
 */

#ifndef EDGEPROXY_H_
#define EDGEPROXY_H_

#include "Edge.h"

namespace lnet
{

template<typename _id>
class EdgeProxy
{
public:
	EdgeProxy(const Edge* e, _id id) : e_(e), id_(id) {}
	EdgeProxy* operator->() { return this; }
	_id id() const { return id_; }
	Node* source() const { return e_->source(); }
	Node* target() const { return e_->target(); }
	bool isLoop() const { return e_->isLoop(); }

private:
	const Edge* e_;
	_id id_;
};

}

#endif /* EDGEPROXY_H_ */
