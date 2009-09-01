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

class MultiEdgeElementFactory: public ElementFactory
{
public:
	MultiEdgeElementFactory() {}
	virtual ~MultiEdgeElementFactory() {}
private:
	MultiNode* doCreateNode(node_id_t id);
	Edge* doCreateEdge(edge_id_t id, Node* source, Node* target);
};

inline MultiNode* MultiEdgeElementFactory::doCreateNode(const node_id_t id)
{
	return new MultiNode(id);
}

inline Edge* MultiEdgeElementFactory::doCreateEdge(const edge_id_t id, Node* source, Node* target)
{
	return new Edge(id, source, target);
}

}

#endif /* MULTIEDGEELEMENTFACTORY_H_ */
