/**
 * @file NetOut.h
 * @author Gerd Zschaler
 * @date Nov 13, 2008
 * Writing networks to streams.
 */
#ifndef NETOUT_H_
#define NETOUT_H_

#include "../base/BasicNetwork.h"
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

	template<class _Network>
	bool put(std::ostream& out, const _Network& net) const;

protected:
	bool writeStates_;

private:
	virtual bool doPut(std::ostream& out, const BasicNetwork& net) const = 0;
};

inline void NetOut::writeStates(const bool v)
{
	writeStates_ = v;
}

inline bool NetOut::writesStates() const
{
	return writeStates_;
}

template<class _Network>
inline bool NetOut::put(std::ostream& out, const _Network& net) const
{
	// make a copy in order to get a contiguous range of valid IDs
	_Network n(net);
	return doPut(out, n);
}

}

}

#endif /* NETOUT_H_ */
