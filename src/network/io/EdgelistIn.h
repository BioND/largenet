/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file EdgelistIn.h
 * @author Gerd Zschaler
 * @date Nov 13, 2008
 * Generating network from edge list file.
 */

#ifndef EDGELISTIN_H_
#define EDGELISTIN_H_

#include "NetIn.h"

namespace lnet
{

class BasicNetwork;

namespace io
{

/**
 * Edge list reader class.
 */
class EdgelistIn: public lnet::io::NetIn
{
public:
	/**
	 * Basic constructor.
	 */
	EdgelistIn();
	/**
	 * Basic destructor.
	 */
	virtual ~EdgelistIn();

	/**
	 * Initialize network from edge list.
	 *
	 * @todo This needs cleaning up, and I don't think it'll work if nodes have
	 * been deleted before writing the edge list (i.e. if the node IDs are not
	 * continuous).
	 *
	 * @param in Stream to read edge list from.
	 * @param net
	 * @return True on success.
	 *
	 * @see EdgelistOut
	 */
	bool get(std::istream& in, BasicNetwork& net);
};

}

}

#endif /* EDGELISTIN_H_ */
