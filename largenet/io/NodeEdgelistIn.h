/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef NODEEDGELISTIN_H_
#define NODEEDGELISTIN_H_

#include <largenet/io/NetIn.h>

namespace lnet
{

namespace io
{

class NodeEdgelistIn: public lnet::io::NetIn
{
public:
	NodeEdgelistIn();
	virtual ~NodeEdgelistIn();
	bool get(std::istream& in, BasicNetwork& net);
};

}

}

#endif /* NODEEDGELISTIN_H_ */
