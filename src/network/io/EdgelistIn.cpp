#include "EdgelistIn.h"
#include "../base/types.h"
#include "../base/BasicNetwork.h"
#include <string>
#include <sstream>
#include <cassert>
#include <utility>
#include <vector>
#include <set>
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
	node_state_size_t node_states = 1;
	std::set<node_id_t> node_ids;

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
			}
			if (!(ss >> b.state))
				b.state = 0;

			while (a.state >= node_states)
				++node_states;
			while (b.state >= node_states)
				++node_states;
		}
		ss.clear();
		node_ids.insert(a.id);
		node_ids.insert(b.id);
		links.push_back(std::make_pair(a, b));
	}

	net.reset(node_ids.size(), links.size(), node_states);
	for (link_vector::const_iterator it = links.begin(); it != links.end(); ++it)
	{
		net.setNodeState(it->first.id, it->first.state);
		net.setNodeState(it->second.id, it->second.state);
		net.addLink(it->first.id, it->second.id);
	}
	return true;
}

}

}
