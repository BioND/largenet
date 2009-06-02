/**
 * @file NetIn.h
 * @author Gerd Zschaler
 * @date Nov 13, 2008
 * Generating networks from input streams.
 */

#ifndef NETIN_H_
#define NETIN_H_

#include <istream>

namespace lnet
{

class BasicNetwork;

/**
 * Namespace containing all network input/output functionality.
 */
namespace io
{

/**
 * Abstract base class for all network readers.
 */
class NetIn
{
public:
	/**
	 * Constructor.
	 */
	NetIn();
	/**
	 * Destructor.
	 */
	virtual ~NetIn();

	/**
	 * Check if reader also reads node and link states from the input stream (if any).
	 * @return true if reader reads states
	 */
	bool readsStates() const;
	/**
	 * Enable/disable state reading.
	 * @param v true if reader should read states
	 */
	void readStates(bool v);

	/**
	 * Read network from input stream. The %MultiNetwork @p net will
	 * be reset before reading.
	 * @param[in] in stream to read from
	 * @param[out] net %MultiNetwork to read to.
	 * @return true on success
	 */
	virtual bool get(std::istream& in, BasicNetwork& net) = 0;

protected:
	bool readStates_; ///< state reading flag
};

inline bool NetIn::readsStates() const
{
	return readStates_;
}

inline void NetIn::readStates(const bool v)
{
	readStates_ = v;
}

}

}

#endif /* NETIN_H_ */
