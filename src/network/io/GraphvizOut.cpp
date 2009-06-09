#include "GraphvizOut.h"
#include "../boost/boost_largenet_multigraph.h"
#include <boost/graph/graphviz.hpp>

namespace lnet
{

namespace io
{

GraphvizOut::GraphvizOut() :
	NetOut()
{
}

GraphvizOut::~GraphvizOut()
{
}

class node_state_writer
{
public:
	node_state_writer(const MultiNetwork& net) :
		net_(net)
	{
	}
	void operator()(std::ostream& out, const node_id_t v) const
	{
		out << " [label=" << net_.nodeState(v) << "]";
	}
private:
	const MultiNetwork& net_;
};

class edge_state_writer
{
public:
	edge_state_writer(const MultiNetwork& net) :
		net_(net)
	{
	}
	void operator()(std::ostream& out, const link_id_t l) const
	{
		out << " [label=" << net_.linkState(l) << "]";
	}
private:
	const MultiNetwork& net_;
};

class graph_info_writer
{
public:
	graph_info_writer(const MultiNetwork& net) :
		net_(net)
	{
	}
	void operator()(std::ostream& out) const
	{
		out << "graph [node_states=" << net_.numberOfNodeStates()
				<< ", edge_states=" << net_.numberOfLinkStates() << "]\n";
	}
private:
	const BasicNetwork& net_;
};

bool GraphvizOut::doPut(std::ostream& out, const MultiNetwork& net) const
{
	if (!out)
		return false;
	if (writeStates_)
	{
		boost::write_graphviz(out, net, node_state_writer(net),
				edge_state_writer(net), graph_info_writer(net));
	}
	else
		boost::write_graphviz(out, net);
	return true;
}

}

}
