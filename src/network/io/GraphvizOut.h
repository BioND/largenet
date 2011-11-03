/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
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
	bool doPut(std::ostream& out, const BasicNetwork& net) const;
};

}

}

#endif /* GRAPHVIZOUT_H_ */
