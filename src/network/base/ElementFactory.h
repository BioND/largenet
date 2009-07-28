/**
 * @file ElementFactory.h
 * @date 24.06.2009
 * @author gerd
 */

#ifndef ELEMENTFACTORY_H_
#define ELEMENTFACTORY_H_

namespace lnet
{

class Node;
class Link;

class ElementFactory
{
public:
	ElementFactory();
	virtual ~ElementFactory();
	virtual Node* createNode() = 0;
	virtual Link* createLink() = 0;

private:
	ElementFactory(const ElementFactory&);
};

inline ElementFactory::ElementFactory()
{
}

inline ElementFactory::~ElementFactory()
{
}

}

#endif /* ELEMENTFACTORY_H_ */
