/**
 * @file GraphvizIn.h
 * @date Dec 11, 2008
 * @author Gerd Zschaler
 */

#ifndef GRAPHVIZIN_H_
#define GRAPHVIZIN_H_

#include "NetIn.h"

namespace lnet
{

namespace io
{

/**
 * Graphviz network reader.
 */
class GraphvizIn: public NetIn
{
public:
	GraphvizIn();
	virtual ~GraphvizIn();
	bool get(std::istream& in, BasicNetwork& net);
};

}

}

#endif /* GRAPHVIZIN_H_ */
