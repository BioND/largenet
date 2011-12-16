#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <largenet.h>
#include <largenet/base/types.h>
#include "SwarmModel.h"

namespace po = boost::program_options;

class Options
{
public:
	Options();

	void parseCommandLine(int argc, char *argv[]);
	void printHelpText(std::ostream& out);
	std::string toStr() const;

	lnet::id_size_t N, L;
	SwarmModel::Params par;
	double l0, linf0, rinf0, lr0, ll0, llinf0, rrinf0;
	bool randomLinks;
	double T;
	double outT, degT, powT;
	unsigned int fileNum;
	bool dropTimeSeries;
	bool avgNN;
	bool diffDist;

	std::string loadNetFile, cfgFile_;
	bool saveNet, compSizes, switchTimes;
	double switchTimesInterval;
	double switchTimesCorridor;
	unsigned long int rngSeed;

private:
	void init();

	po::options_description allOptions_;
	po::variables_map vm_;
};

std::ostream& operator<<(std::ostream &out, const Options& o);

#endif /* OPTIONS_H_ */
