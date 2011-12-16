/* Initializes the network, defines output objects and assigns them to output files,
 * Creates the SwarmModel and defines the simulation loop
 */

#include "SwarmApp.h"
#include "SwarmModel.h"
#include "output/TimeSeriesOutput.h"
#include "output/DegDistOutput.h"
#include "output/DiffDegDistOutput.h"
#include "output/NodeStateOutput.h"
#include "output/StateIntervalOutput.h"
#include "output/SuggestionPower.h"
#include <boost/program_options.hpp>
#include <cmath>
#include <largenet/generators.h>
#include <largenet/io/EdgelistIn.h>
#include <largenet/measures/component.h>
#include <largenet/myrng1.2/myrngWELL.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

using namespace lnet;
using namespace std;

SwarmApp::SwarmApp(int argc, char** argv)
{
	/* Using the command line parsing, assigns the values to the parameters and options
	 * Then creates the TripleNetwork object from the largenet library  which is the network that keeps all info of nodes and links
	 * Creates the SwarmModel object which defines the network processes using the options
	 * If an external network is given, tries to load it
	 */
	try
	{
		opts_.parseCommandLine(argc, argv);
		if (opts_.rngSeed > 0)
			rng.Seed(opts_.rngSeed);

		net_ = auto_ptr<TripleNetwork> (new TripleNetwork(opts_.N, opts_.L,	SwarmModel::node_states));

		model_ = auto_ptr<SwarmModel> (new SwarmModel(*net_, opts_.par));
		if (!opts_.loadNetFile.empty())
			loadNetFile();
		else
			init();
	} catch (boost::program_options::error& e)
	{
		cerr << e.what() << "\n";
		opts_.printHelpText(cout);
		exit(1);
	} catch (std::exception& e)
	{
		cerr << e.what() << "\n";
		exit(1);
	}
}

SwarmApp::~SwarmApp()
{
	// no need for this with auto_ptr
	//	delete model_;
	//	delete net_;
	for (ofstream_ptr_v::iterator i = outStreams_.begin(); i!= outStreams_.end(); ++i)
		i->close();
}

void SwarmApp::initNodeStates()
{

	/* Initializes the node states
	 * Checks if options are correct
	 */
	assert(opts_.linf0 >= 0);
	assert(opts_.linf0 <= 1.0);
	assert(opts_.rinf0 >= 0);
	assert(opts_.rinf0 <= 1.0);
	assert(opts_.l0 >= 0);
	assert(opts_.l0 <= 1.0);

	/* First assigns R_r state to all nodes
	 */

	TripleNetwork::NodeIteratorRange iters = net_->nodes();
	TripleNetwork::NodeIterator it = iters.first;
	while (it!= iters.second)
	{
		net_->setNodeState(*it, SwarmModel::Rrinf);
		++it;
	}

	/* Then L_l states
	 */
	const id_size_t initialLeftinformedNodes = static_cast<id_size_t> (round(net_->numberOfNodes() * opts_.linf0));
	TripleNetwork::NodeIteratorRange iters2 = net_->nodes();
	TripleNetwork::NodeIterator it2 = iters2.first;
	while ((net_->numberOfNodes(SwarmModel::Llinf) < initialLeftinformedNodes) && (it2!= iters2.second))
	{
		net_->setNodeState(*it2, SwarmModel::Llinf);
		++it2;
	}

	/* Then L_u states
	 */
	const id_size_t initialLeftuninformedNodes = static_cast<id_size_t> (round(0.5*net_->numberOfNodes() * (1-opts_.linf0-opts_.rinf0)));
	TripleNetwork::NodeIteratorRange iters3 = net_->nodes();
	TripleNetwork::NodeIterator it3 = iters3.first;
	while ((net_->numberOfNodes(SwarmModel::Luninf) < initialLeftuninformedNodes) && (it3!= iters3.second))
	{
		if(net_->nodeState(*it3)==SwarmModel::Rrinf)
			net_->setNodeState(*it3, SwarmModel::Luninf);
		++it3;
	}

	/* Then R_u states
	 */
	const id_size_t initialRightuninformedNodes = static_cast<id_size_t> (round(0.5*net_->numberOfNodes() * (1-opts_.linf0-opts_.rinf0)));
	TripleNetwork::NodeIteratorRange iters4 = net_->nodes();
	TripleNetwork::NodeIterator it4 = iters4.first;
	while ((net_->numberOfNodes(SwarmModel::Runinf) < initialRightuninformedNodes) && (it4!= iters4.second))
	{
		if(net_->nodeState(*it4)==SwarmModel::Rrinf)
			net_->setNodeState(*it4, SwarmModel::Runinf);
		++it4;
	}
}


void SwarmApp::init()
{
	/*Initializes the network
	 * First initializes the node states
	 * If links are assumed to be random assigns them appropriately
	 * Otherwise adds random links preserving the assumed densities of links
	 */
	initNodeStates();
	if (opts_.randomLinks)
	{
		generators::randomNetworkGnm(*net_, opts_.L);
	}
	else
	{
		const id_size_t nlr = static_cast<id_size_t> (round(net_->numberOfNodes() * opts_.lr0));
		const id_size_t nll = static_cast<id_size_t> (round(net_->numberOfNodes() * opts_.ll0));
		const id_size_t nrr = opts_.L - nlr - nll;
		addRandomLinks(nrr, SwarmModel::Runinf, SwarmModel::Runinf);
		addRandomLinks(nll, SwarmModel::Luninf, SwarmModel::Luninf);
		addRandomLinks(nlr, SwarmModel::Luninf, SwarmModel::Runinf);
	}

}

void SwarmApp::addRandomLinks(const id_size_t n, const node_state_t a,	const node_state_t b)
{
	/*Creates n random links between nodes of state a and b
	 * Self and double links are not allowed
	 */
	if (net_->numberOfNodes() <= 1)
		return;
	link_state_t s = net_->linkStateCalculator()(a, b);
	while (net_->numberOfLinks(s) < n)
	{
		pair<bool, node_id_t> na = net_->randomNode(a), nb =
				net_->randomNode(b);
		if (na.second == nb.second || net_->isLink(na.second,nb.second).first|| net_->isLink(nb.second,na.second).first)
			continue;
		net_->addLink(na.second, nb.second);
	}
}

void SwarmApp::simLoop()
{
	/* The simulation loop
	 * Iterates the step function of the SwarmModel object in which one of the network processes is realized
	 */
	double t = 0;
	while (t <= opts_.T)
	{
		outputter_.output(t);
		t += model_->step(t);
		//		++events;
	}
}

void SwarmApp::loadNetFile()
{
	/*Loads a network from a given file
	 * Reads nodes, links, and states if possible
	 * If nodes are not assigned states, initializes them
	 *  Otherwise, sets system parameters using the network data
	 */
	ifstream fin(opts_.loadNetFile.c_str());
	if (!fin)
	{
		fin.close();
		throw(std::invalid_argument("Cannot open '" + opts_.loadNetFile
				+ "' for reading."));
	}

	io::EdgelistIn ei;
	ei.get(fin, *net_);
	fin.close();

	assert(net_->numberOfNodeStates() == SwarmModel::node_states);
	assert(net_->numberOfLinkStates() == SwarmModel::link_states);
	opts_.N = net_->numberOfNodes();
	opts_.L = net_->numberOfLinks();
	if (!ei.readsStates())
		initNodeStates();
	else
	opts_.l0 = static_cast<double> (net_->numberOfNodes(SwarmModel::Luninf)+net_->numberOfNodes(SwarmModel::Llinf)+net_->numberOfNodes(SwarmModel::Lrinf))/ net_->numberOfNodes();
	opts_.ll0 = static_cast<double> (net_->numberOfLinks(SwarmModel::LuninfLuninf)+net_->numberOfLinks(SwarmModel::LuninfLlinf)+net_->numberOfLinks(SwarmModel::LuninfLrinf)+net_->numberOfLinks(SwarmModel::LlinfLlinf)+net_->numberOfLinks(SwarmModel::LlinfLrinf)+net_->numberOfLinks(SwarmModel::LrinfLrinf))	/ net_->numberOfNodes();
	opts_.lr0 = static_cast<double> (net_->numberOfLinks(SwarmModel::LuninfRuninf)+net_->numberOfLinks(SwarmModel::LuninfRlinf)+net_->numberOfLinks(SwarmModel::LuninfRrinf)+net_->numberOfLinks(SwarmModel::LlinfRuninf)+net_->numberOfLinks(SwarmModel::LlinfRlinf)+net_->numberOfLinks(SwarmModel::LlinfRrinf)+net_->numberOfLinks(SwarmModel::LrinfRuninf)+net_->numberOfLinks(SwarmModel::LrinfRlinf)+net_->numberOfLinks(SwarmModel::LrinfRrinf))	/ net_->numberOfNodes();
	opts_.linf0 = static_cast<double> (net_->numberOfNodes(SwarmModel::Llinf)+net_->numberOfNodes(SwarmModel::Rlinf))/ net_->numberOfNodes();
	opts_.rinf0 = static_cast<double> (net_->numberOfNodes(SwarmModel::Lrinf)+net_->numberOfNodes(SwarmModel::Rrinf))/ net_->numberOfNodes();
}

string SwarmApp::getFileName(const string tag, unsigned int num) const
{
	// Reads the file name
	stringstream ss;

	ss << appName_;
	if (!tag.empty())
		ss << "-" << tag;
	ss << "-" << opts_.toStr();
	if (num > 0)
		ss << num;
	ss << ".data";
	return ss.str();
}

ofstream& SwarmApp::openOutputStream(const string tag)
{
	//Creates an output stream with a given name

	string f(getFileName(tag));
	auto_ptr<ofstream> o(new ofstream(f.c_str()));
	if (o->bad())
	{
		o->close();
		throw("Could not open " + f + " for writing!\n");
	}
	outStreams_.push_back(o);
	return outStreams_.back();
}

int SwarmApp::exec()
{
	/* a positive simulation length is required
	 */
	if (opts_.T == 0)
		return 0;

	/* Adds an output stream for time series of node, link, and triplet densities
	 */
	if (!opts_.dropTimeSeries)
	{
		ofstream& tsOutStream = openOutputStream();
		outputter_.addOutput(new TimeSeriesOutput(tsOutStream, *net_,opts_.outT));
	}

	/* Adds an output stream for degree distributions
	 */
	if (opts_.degT > 0)
	{
		ofstream& degdistOutStream = openOutputStream("degree");
		degdistOutStream << opts_;
		if (opts_.diffDist)
			outputter_.addOutput(new DiffDegDistOutput(degdistOutStream, *net_,
					opts_.degT, true));
		else
			outputter_.addOutput(new DegDistOutput(degdistOutStream, *net_,
					opts_.degT));
	}
	/* Calls the iteration loop
	 */
	simLoop();

	/*writeSwitchEvents();
	writeAlignEvents();
	if (opts_.compSizes)
		writeComponentSizes();*/

	return 0;
}

void SwarmApp::writeSwitchEvents()
{
	/* After the simulation, prints the state switching events
	 */
	ofstream& o = openOutputStream("switch");
	o << "#t \t new_state \t maj \n";
	for (event_list::const_iterator i = model_->switchEvents().begin(); i != model_->switchEvents().end(); ++i)
		o << i->time() << '\t' << i->newState() << '\t' << i->maj() << '\n';
}

void SwarmApp::writeAlignEvents()
{
	/* After the simulation, prints the state alignment events
	 */
	ofstream& o = openOutputStream("align");
	o << "#t \t new_state \t maj \t inf\n";
	for (align_event_list::const_iterator i = model_->alignEvents().begin(); i != model_->alignEvents().end(); ++i)
		o << i->time() << '\t' << i->newState() << '\t' << i->maj() << '\t' << i->inf() << '\n';
}

void SwarmApp::writeComponentSizes()
{
	/* Prints the size of all network components
	 */
	measures::component_vector comps;
	measures::connectedComponents(*net_, comps);
	ofstream& o = openOutputStream("csizes");
	o << "# Component sizes\n";
	for (measures::component_vector::iterator i = comps.begin(); i
			!= comps.end(); ++i)
		o << i->size() << "\n";
}
