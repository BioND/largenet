/**
 * @file MultiEdgeElementFactory.h
 * @date 01.09.2009
 * @author gerd
 */

#ifndef MULTIEDGEELEMENTFACTORY_H_
#define MULTIEDGEELEMENTFACTORY_H_

#include "ElementFactory.h"
#include "MultiNode.h"
#include "Edge.h"

namespace largenet
{

template<class EdgeType = UndirectedEdge>
class MultiEdgeElementFactory: public ElementFactory
{
public:
	MultiEdgeElementFactory() {}
	virtual ~MultiEdgeElementFactory() {}
private:
	MultiNode* doCreateNode(node_id_t id);
	EdgeType* doCreateEdge(edge_id_t id, Node* source, Node* target);
};

template<class EdgeType>
inline MultiNode* MultiEdgeElementFactory<EdgeType>::doCreateNode(const node_id_t id)
{
	return new MultiNode(id);
}

template<class EdgeType>
inline EdgeType* MultiEdgeElementFactory<EdgeType>::doCreateEdge(const edge_id_t id, Node* source, Node* target)
{
	return new EdgeType(id, source, target);
}

}

#endif /* MULTIEDGEELEMENTFACTORY_H_ */
