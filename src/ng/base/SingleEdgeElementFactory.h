/**
 * @file SingleEdgeElementFactory.h
 * @date 18.08.2009
 * @author gerd
 */

#ifndef SINGLEEDGEELEMENTFACTORY_H_
#define SINGLEEDGEELEMENTFACTORY_H_

#include "ElementFactory.h"
#include "SingleNode.h"
#include "Edge.h"

namespace largenet
{

class SingleEdgeElementFactory: public ElementFactory
{
public:
	SingleEdgeElementFactory() {}
	virtual ~SingleEdgeElementFactory() {}
private:
	Node* doCreateNode(node_id_t id);
	Edge* doCreateEdge(edge_id_t id, Node* source, Node* target);
};

inline Node* SingleEdgeElementFactory::doCreateNode(const node_id_t id)
{
	return new SingleNode(id);
}

inline Edge* SingleEdgeElementFactory::doCreateEdge(const edge_id_t id, Node* source,
		Node* target)
{
	return new Edge(id, source, target);
}
}

#endif /* SINGLEEDGEELEMENTFACTORY_H_ */
