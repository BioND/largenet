/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
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
