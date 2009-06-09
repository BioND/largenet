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
private:
	bool doPut(std::ostream& out, const MultiNetwork& net) const;
};

}

}

#endif /* GRAPHVIZOUT_H_ */
