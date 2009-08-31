/**
 * @file SingleEdgeElementFactory.h
 * @date 18.08.2009
 * @author gerd
 */

#ifndef SINGLEEDGEELEMENTFACTORY_H_
#define SINGLEEDGEELEMENTFACTORY_H_

#include "ElementFactory.h"
#include "Node.h"
#include "Edge.h"

namespace lnet
{

template<class G>
class SingleEdgeElementFactory: public ElementFactory<G>
{
public:
	typedef typename ElementFactory<G>::NodeType NodeType;
	typedef typename ElementFactory<G>::EdgeType EdgeType;
	typedef typename ElementFactory<G>::node_descriptor node_descriptor;
	typedef typename ElementFactory<G>::edge_descriptor edge_descriptor;
	SingleEdgeElementFactory() {}
	virtual ~SingleEdgeElementFactory() {}
private:
	NodeType* doCreateNode(node_descriptor id);
	EdgeType* doCreateEdge(edge_descriptor id, NodeType* source,
			NodeType* target);
};

template<class G>
inline typename SingleEdgeElementFactory<G>::NodeType* SingleEdgeElementFactory<
		G>::doCreateNode(const node_descriptor id)
{
	return new SingleNode<G> (id);
}

template<class G>
inline typename SingleEdgeElementFactory<G>::EdgeType* SingleEdgeElementFactory<
		G>::doCreateEdge(const edge_descriptor id, NodeType* source,
		NodeType* target)
{
	return new Edge<G> (id, source, target);
}
}

#endif /* SINGLEEDGEELEMENTFACTORY_H_ */
