/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#include "NodeEdgelistIn.h"
#include "iodefs.h"
#include "../base/types.h"
#include "../base/BasicNetwork.h"
#include <string>
#include <sstream>

#include <iostream>
#include <cassert>

namespace lnet
{

namespace io
{

using namespace std;

NodeEdgelistIn::NodeEdgelistIn() :
	NetIn()
{
}

NodeEdgelistIn::~NodeEdgelistIn()
{
}

bool NodeEdgelistIn::get(istream& in, BasicNetwork& net)
{
	/** @todo implement me. */
	assert(false);
	/*
	if (!in)
		return false;

	string line;
	stringstream ss;
	id_size_t N, L, node_states = 1, link_states = 1;

	// read number of nodes and links
	std::getline(in, line);
	line += " ";
	ss.str(line);
	ss >> skipws >> N >> L;
	if (!ss.fail())
	{
		if (!(ss >> node_states))
			node_states = 1;
		if (!(ss >> link_states))
			link_states = 1;
	}
	ss.clear();

	net.reset(N, L, node_states, link_states);

	// read separator
	string tok;

	std::getline(in, line);
	line += " ";
	ss.str(line);
	ss >> skipws >> tok;
	if (tok != NODE_LIST) throw("No node list token."); // FIXME
*/
// TODO
	return true;
}

}

}
