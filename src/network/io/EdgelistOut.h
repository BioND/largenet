/**
 * @file EdgelistOut.h
 * @author Gerd Zschaler
 * @date Nov 13, 2008
 * Writing networks to edge list files.
 */

#ifndef EDGELISTOUT_H_
#define EDGELISTOUT_H_

#include "NetOut.h"

namespace lnet
{

namespace io
{

/**
 * Edge list writer class.
 */
class EdgelistOut: public lnet::io::NetOut
{
public:
	/**
	 * Basic constructor.
	 */
	EdgelistOut();
	/**
	 * Basic destructor.
	 */
	virtual ~EdgelistOut();

	/**
	 * Write edge list to file. The first line is
	 * @code
	 * int1 int2 int3 int4
	 * @endcode
	 * where @c int1 gives the number of nodes, @c int2 the number of links, @c int3
	 * the number of different node states, and @c int4 the number of link states.
	 * The following lines have the format:
	 * @code
	 * int1 int2 int3 int4 int5
	 * @endcode
	 * where @c int1 and @c int2 are the node IDs connected by an edge, @c int3 and @c int4
	 * are there respective states, and @c int5 is the state associated with the link.
	 *
	 * @param[in] out Stream to write edge list to.
	 * @param[out] net
	 *
	 * @see EdgelistIn
	 *
	 * @warning This *only* writes edges with associated states to the stream. Thus,
	 * state information of isolated vertices will get lost! When building a network
	 * from such edge list data via EdgelistIn, isolated vertices' states will be
	 * default-constructed.
	 */
	bool put(std::ostream& out, const MultiNetwork& net);
};

}

}

#endif /* EDGELISTOUT_H_ */
