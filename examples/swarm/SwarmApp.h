#ifndef SWARMAPP_H_
#define SWARMAPP_H_

#include "Options.h"
#include "output/Outputter.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <memory> // for auto_ptr
#include <string>
#include <fstream>

class SwarmModel;

class SwarmApp
{
public:
	SwarmApp(int argc, char** argv);
	~SwarmApp();
	void setAppName(std::string name);
	void setAppName(const char* name);
	std::string appName() const;
	int exec();

private:
	void init();
	void initNodeStates();
	void loadNetFile();
	void addRandomLinks(lnet::id_size_t n, lnet::node_state_t a, lnet::node_state_t b);
	void simLoop();
	std::string getFileName(std::string tag = "", unsigned int num = 0) const;
	std::ofstream& openOutputStream(std::string tag = "");
	void writeSwitchEvents();
	void writeAlignEvents();
	void writeComponentSizes();

	Options opts_;
	std::auto_ptr<lnet::TripleNetwork> net_;
	std::auto_ptr<SwarmModel> model_;
	std::string appName_;
	Outputter outputter_;
	typedef boost::ptr_vector<std::ofstream> ofstream_ptr_v;
	ofstream_ptr_v outStreams_;
};

inline void SwarmApp::setAppName(std::string name)
{
	appName_ = name;
}

inline void SwarmApp::setAppName(const char* name)
{
	appName_ = name;
}

inline std::string SwarmApp::appName() const
{
	return appName_;
}

#endif /* SWARMAPP_H_ */
