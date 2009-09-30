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

template<class EdgeType = UndirectedEdge>
class SingleEdgeElementFactory: public ElementFactory
{
public:
	SingleEdgeElementFactory() {}
	virtual ~SingleEdgeElementFactory() {}
private:
	SingleNode* doCreateNode(node_id_t id);
	EdgeType* doCreateEdge(edge_id_t id, Node* source, Node* target);
};

template<class EdgeType>
inline SingleNode* SingleEdgeElementFactory<EdgeType>::doCreateNode(const node_id_t id)
{
	return new SingleNode(id);
}

template<class EdgeType>
inline EdgeType* SingleEdgeElementFactory<EdgeType>::doCreateEdge(const edge_id_t id, Node* source,
		Node* target)
{
	return new EdgeType(id, source, target);
}
}

#endif /* SINGLEEDGEELEMENTFACTORY_H_ */
