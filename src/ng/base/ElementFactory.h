/**
 * @file ElementFactory.h
 * @date 18.08.2009
 * @author gerd
 */

#ifndef ELEMENTFACTORY_H_
#define ELEMENTFACTORY_H_

#include "traits.h"

namespace lnet
{

template<class G> class Node;
template<class G> class Edge;
//class Triple;

template<class G>
class ElementFactory
{
public:
	typedef typename graph_traits<G>::node_type NodeType;
	typedef typename graph_traits<G>::edge_type EdgeType;
	typedef typename graph_traits<G>::node_descriptor node_descriptor;
	typedef typename graph_traits<G>::edge_descriptor edge_descriptor;

	ElementFactory() {}
	virtual ~ElementFactory() {}
	NodeType* createNode(node_descriptor id) { return doCreateNode(id); }
	EdgeType* createEdge(edge_descriptor id, NodeType* source, NodeType* target) { return doCreateEdge(id, source, target); }
	//Triple* createTriple(edge_descriptor left, edge_descriptor right);	FIXME directed triples?
private:
	virtual NodeType* doCreateNode(node_descriptor id) = 0;
	virtual EdgeType* doCreateEdge(edge_descriptor id, NodeType* source, NodeType* target) = 0;
};

}

#endif /* ELEMENTFACTORY_H_ */
