/**
 * @file EdgelistIn.h
 * @author Gerd Zschaler
 * @date Nov 13, 2008
 * Generating network from edge list file.
 */

#ifndef EDGELISTIN_H_
#define EDGELISTIN_H_

#include "NetIn.h"
#include "../MultiNetwork.h"

namespace lnet
{

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
	bool get(std::istream& in, MultiNetwork& net);
};

}

}

#endif /* EDGELISTIN_H_ */
