/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
#include "EdgelistIn.h"
#include <largenet/base/types.h>
#include <largenet/base/BasicNetwork.h>
#include <string>
#include <sstream>
#include <cassert>
#include <utility>
#include <vector>
#include <cstdlib>

namespace lnet
{

namespace io
{

using namespace std;

EdgelistIn::EdgelistIn() :
	NetIn()
{
}

EdgelistIn::~EdgelistIn()
{
}

struct node_info
{
	node_id_t id;
	node_state_t state;
};

typedef std::pair<node_info, node_info> link_info;
typedef std::vector<link_info> link_vector;

bool EdgelistIn::get(std::istream& in, BasicNetwork& net)
{
	if (!in)
		return false;

	string line;
	stringstream ss;

	link_vector links;
	node_info a, b;
	node_state_size_t node_states = readStates_ ? 1 : net.numberOfNodeStates();
	node_id_t max_node_id = 0;

	while (std::getline(in, line))
	{
		line += " ";
		ss.str(line);
		ss >> a.id >> b.id;
		if (readStates_ && (!ss.fail()))
		{
			if (!(ss >> a.state))
			{
				a.state = 0;
				readStates_ = false; // failed on reading state, so do not continue reading states
				// do not reduce number of node states if failed to read states
				if (node_states < net.numberOfNodeStates())
					node_states = net.numberOfNodeStates();
			}
			if (!(ss >> b.state))
				b.state = 0;

			while (a.state >= node_states)
				++node_states;
			while (b.state >= node_states)
				++node_states;
		}
		ss.clear();
		if (a.id > max_node_id) max_node_id = a.id;
		if (b.id > max_node_id) max_node_id = b.id;
		links.push_back(std::make_pair(a, b));
	}

	net.reset(max_node_id + 1, links.size(), node_states);
	for (link_vector::iterator it = links.begin(); it != links.end(); ++it)
	{
		net.setNodeState(it->first.id, it->first.state);
		net.setNodeState(it->second.id, it->second.state);
		net.addLink(it->first.id, it->second.id);
	}
	return true;
}

}

}
