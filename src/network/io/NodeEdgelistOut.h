/*
 * NodeEdgelistOut.h
 *
 *  Created on: 16.04.2009
 *      Author: gerd
 */

#ifndef NODEEDGELISTOUT_H_
#define NODEEDGELISTOUT_H_

#include "NetOut.h"
#include <iostream>

namespace lnet
{

namespace io
{

class NodeEdgelistOut: public lnet::io::NetOut
{
public:
	NodeEdgelistOut();
	virtual ~NodeEdgelistOut();
private:
	bool doPut(std::ostream& out, const BasicNetwork& net) const;
};

}

}

#endif /* NODEEDGELISTOUT_H_ */
