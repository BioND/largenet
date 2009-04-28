/*
 * NodeEdgelistIn.h
 *
 *  Created on: 28.04.2009
 *      Author: gerd
 */

#ifndef NODEEDGELISTIN_H_
#define NODEEDGELISTIN_H_

#include "NetIn.h"

namespace lnet
{

class MultiNetwork;

namespace io
{

class NodeEdgelistIn: public lnet::io::NetIn
{
public:
	NodeEdgelistIn();
	virtual ~NodeEdgelistIn();
	bool get(std::istream& in, MultiNetwork& net);
};

}

}

#endif /* NODEEDGELISTIN_H_ */
