/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef NODEEDGELISTOUT_H_
#define NODEEDGELISTOUT_H_

#include <largenet/io/NetOut.h>
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
