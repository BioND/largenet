#include "EdgelistIn.h"
#include "../base/types.h"
#include "../base/BasicNetwork.h"
#include <string>
#include <sstream>
#include <cassert>

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

bool EdgelistIn::get(std::istream& in, BasicNetwork& net)
{
	if (!in)
		return false;

	string line;
	stringstream ss;
	id_size_t N, L, node_states = 1, link_states = 1;

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

	node_id_t a, b;
	node_state_t sa, sb;
	link_state_t sl;

	while (std::getline(in, line))
	{
		line += " ";
		ss.str(line);
		ss >> a >> b;
		sa = sb = sl = 0;
		if (readStates_ && (!ss.fail()))
		{
			if (!(ss >> sa))
			{
				sa = 0;
				readStates_ = false; // failed on reading state, so do not continue reading states
			}
			if (!(ss >> sb))
				sb = 0;
			if (!(ss >> sl))
				sl = 0;
		}
		ss.clear();
		net.setNodeState(a, sa); // FIXME This might fail if a node state in the edge list file
		// is larger than the (erroneously not supplied) number of node states
		net.setNodeState(b, sb);
		net.addLink(a, b);
	}
	return true;
}

}

}
