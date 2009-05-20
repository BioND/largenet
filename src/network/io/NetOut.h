/**
 * @file NetOut.h
 * @author Gerd Zschaler
 * @date Nov 13, 2008
 * Writing networks to streams.
 */
#ifndef NETOUT_H_
#define NETOUT_H_

#include "../MultiNetwork.h"
#include <ostream>

namespace lnet
{

namespace io
{

/**
 * Abstract base class for all network writers.
 */
class NetOut
{
public:
	NetOut();
	virtual ~NetOut();

	bool writesStates() const;
	void writeStates(bool v);

	virtual bool put(std::ostream& out, const MultiNetwork& net) = 0;

protected:
	bool writeStates_;
};


inline void NetOut::writeStates(const bool v)
{
	writeStates_ = v;
}

inline bool NetOut::writesStates() const
{
	return writeStates_;
}

}

}

#endif /* NETOUT_H_ */
