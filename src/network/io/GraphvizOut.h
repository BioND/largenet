/**
 * @file GraphvizOut.h
 * @date Dec 10, 2008
 * @author Gerd Zschaler
 */

#ifndef GRAPHVIZOUT_H_
#define GRAPHVIZOUT_H_

#include "NetOut.h"

namespace lnet
{

namespace io
{

/**
 * Graphviz network writer.
 */
class GraphvizOut: public NetOut
{
public:
	GraphvizOut();
	virtual ~GraphvizOut();
	bool put(std::ostream& out, const MultiNetwork& net);
};

}

}

#endif /* GRAPHVIZOUT_H_ */
