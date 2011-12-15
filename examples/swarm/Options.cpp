/* Defines and gives instructions on the parameters, initial conditions, and simulation related options.
 * Defines how to parse the command line and read into the parameters
 * Prints the parameter values
*/
#include "Options.h"
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <boost/any.hpp>

using namespace std;

Options::Options() : loadNetFile(""), cfgFile_(""),
	allOptions_("Allowed options")
{
	init();
}

void Options::init()
{
	/*To be read from the command line, provides a help interface which can be seen by typing -h
	 * Assign default values and overloads them if any value is entered by the user
	 * Parameters define the process rates, main network quantities: initial number of nodes and links
	 * Initial conditions are the initial node and link densities
	 * Simulation options sets how long to simulate, which type of outputs in which frequency to provide
	 * Additional options are defined for loading network data from an external source and saving network to file
	 * */
	po::options_description parOpts("Parameters"), iniOpts("Initial conditions"), simOpts("Simulation options");
	parOpts.add_options()
		("nodes,n", po::value<lnet::id_size_t>(&N)->default_value(1000), "Number of nodes.")
		("links,l", po::value<lnet::id_size_t>(&L)->default_value(2000), "Initial number of links.")
		("switching_uninformed,q", po::value<double>(&par.q)->default_value(0.005), "Spontaneous rate of L <-> R switching.")
		("switching_informed_left", po::value<double>(&par.QL)->default_value(0.005), "Spontaneous rate of informed R -> L switching.")
		("switching_informed_right", po::value<double>(&par.QR)->default_value(0.005), "Spontaneous rate of informed L -> R switching.")
		("conviction,c", po::value<double>(&par.w2)->default_value(0.01), "Rate of LR -> LL/RR transformations")
		("persuasion,p", po::value<double>(&par.w3)->default_value(0.01), "Rate of RLR/LRL -> RRR/LLL transformations")
		("a-different,a", po::value<double>(&par.ad)->default_value(0.05), "LR link creation rate")
		("d-different,b", po::value<double>(&par.dd)->default_value(0.05), "LR link destruction rate")
		("a-equal", po::value<double>(&par.ae)->default_value(0.0), "LL/RR link creation rate")
		("d-equal", po::value<double>(&par.de)->default_value(0.0), "LL/RR link destruction rate");

	iniOpts.add_options()
		("left_total", po::value<double>(&l0)->default_value(1.0), "Initial fraction of uninformed L nodes")
		("leftinf", po::value<double>(&linf0)->default_value(0.0), "Initial fraction of left-informed L nodes")
		("rightinf", po::value<double>(&rinf0)->default_value(0.0), "Initial fraction of right-informed R nodes")
		("lrlinks", po::value<double>(&lr0)->default_value(-1.0), "Initial ratio of LR links.")
		("lllinks", po::value<double>(&ll0)->default_value(-1.0), "Initial ratio of LL links.");

	simOpts.add_options()
		("timeframe,t", po::value<double>(&T)->default_value(3000), "Maximum simulation time.")
		("no-time-series,s", po::value<bool>(&dropTimeSeries)->zero_tokens()->default_value(false), "Do not output time series.")
		("interval,i", po::value<double>(&outT)->default_value(0.5), "Time interval to generate output at.")
		("distint,d", po::value<double>(&degT)->default_value(30), "Time interval to generate degree distribution at (0 disables).")
		("powint,g", po::value<double>(&powT)->default_value(0), "Time interval to generate suggestion power at (0 disables).")
		("diff-dist", po::value<bool>(&diffDist)->zero_tokens()->default_value(false), "Calculate differential degree distributions.")
		("avg-neighbor-deg", po::value<bool>(&avgNN)->zero_tokens()->default_value(false), "Calculate average degree of nearest neighbors.")
		("components", po::value<bool>(&compSizes)->zero_tokens()->default_value(false), "Calculate sizes of connected components.")
		("switch-times", po::value<bool>(&switchTimes)->zero_tokens()->default_value(false), "Measure switching times.")
		("switch-times-corridor", po::value<double>(&switchTimesCorridor)->default_value(0), "Threshold corridor around [R] = 0.5 for switch time measurement.")
		("switch-time-interval", po::value<double>(&switchTimesInterval)->default_value(0), "Switching times sampling interval.");

	allOptions_.add_options()
		("help,h", "This help message.")
		("conf", po::value<string>(&cfgFile_)->default_value(""), "Load configuration file.")
		("filenum,m", po::value<unsigned int>(&fileNum)->default_value(0), "Append number to output file.")
		("load", po::value<string>(&loadNetFile)->default_value(""), "Try to load network from file.")
		("save", po::value<bool>(&saveNet)->zero_tokens()->default_value(false), "Save network to file.")
		("rnd-seed", po::value<unsigned long int>(&rngSeed)->default_value(0), "Random number generator seed.");

	allOptions_.add(parOpts).add(iniOpts).add(simOpts);

	randomLinks = (lr0 < 0) || (ll0 < 0);
	l0=0.5*(1-linf0-rinf0)+linf0;
	rngSeed = 0;
	if (randomLinks)
	{
		const double rho = static_cast<double> (L) / N;
		ll0 = l0 * l0 * rho;
		lr0 = 2*rho*l0*(1.0 - l0);
	}
}

void Options::parseCommandLine(int argc, char* argv[])
{
	/* Options given by the user from the command line are assigned to the variables by parsing the command line using the definitions above
	 */
	po::store(po::parse_command_line(argc, argv, allOptions_), vm_);
	po::notify(vm_);
	if (vm_.count("help"))
	{
		throw po::error("");
	}
	if (!cfgFile_.empty())
	{
		ifstream fin(cfgFile_.c_str());
		if (fin)
		{
			po::store(po::parse_config_file(fin, allOptions_), vm_);
			po::notify(vm_);
		}
		fin.close();
	}
	/* Sets link densities to random values if they are assumed to be random (default value)
	 *
	 */
	l0=0.5*(1-linf0-rinf0)+linf0;
	randomLinks = (lr0 < 0) || (ll0 < 0);
	if (randomLinks)
	{
		const double rho = static_cast<double> (L) / N;
		ll0 = l0 * l0 * rho;
		lr0 = 2*rho*l0*(1.0 - l0);
	}
}

void Options::printHelpText(ostream& out)
{
	/* Prints the definition of all options to the command line
	 */
	out << allOptions_ << "\n";
}

string Options::toStr() const
{
	string s(str(boost::format("n%1%-l%2%-q%3%-QL%4%-QR%5%-c%6%-p%7%-ad%8%-dd%9%-ae%10%-de%11%--L%12%--linf%13%--rinf%14%")
					% N % L % par.q % par.QL % par.QR % par.w2 % par.w3 % par.ad % par.dd % par.ae % par.de % l0 % linf0 % rinf0));

	if (!randomLinks)
		s += str(boost::format("--LR%1%--LL%2%") % lr0 % ll0);

	s += str(boost::format("-t%1%-i%2%--g%3%") % T % outT % powT);

	if (switchTimes)
		s += str(boost::format("--sc%1%") % switchTimesCorridor);

	if (fileNum > 0)
		s += str(boost::format("-m%1%") % fileNum);

	return s;
}

ostream& operator<<(ostream &out, const Options& o)
{
	/* Provides the assumed values of options
	 */
	out << right;
	out << "# Adaptive network model of left-right swarm system with N = " << o.N << " nodes and L = "
			<< o.L << " links.\n"
			<< "# Uninformed spontaneous switching rate q = " << o.par.q << "\n"
			<< "# Informed spontaneous left switching rate QL = " << o.par.QL << "\n"
			<< "# Informed spontaneous right switching rate QR = " << o.par.QR << "\n"
			<< "# Conviction rate w2 (c) = " << o.par.w2 << "\n"
			<< "# Persuasion rate w3 (p) = " << o.par.w3 << "\n"
			<< "# L-R link creation rate ad = " << o.par.ad << "\n"
			<< "# L-R link destruction rate dd = " << o.par.dd << "\n"
			<< "# L-L/R-R link creation rate ae = " << o.par.ae << "\n"
			<< "# L-L/R-R link destruction rate de = " << o.par.de << "\n"
			<< "\n#\n# Initial conditions:\n" << "# L_total = " << setprecision(5)
            << o.l0 << setprecision(5) <<  ", Left informed = " << setprecision(5)  << o.linf0
            << setprecision(5) << ", Right informed = " << setprecision(5)  << o.rinf0
            << setprecision(5) << ", LR = " << setprecision(5)
            << o.lr0 << setprecision(5) << ", LL = " << setprecision(5) << o.ll0
			<< "\n" << "#\n# Simulation time T = " << o.T << "; output every "
			<< o.outT << " time units.\n"
			<< "# Suggestion power calculated every " << o.powT << " time units.\n"
			<< "# Degree distribution calculated every " << o.degT
			<< " time units.\n";
	return out;
}
