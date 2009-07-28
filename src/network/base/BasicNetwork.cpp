/**
 * @file BasicNetwork.cpp
 * @date 21.06.2009
 * @author gerd
 */

#include "BasicNetwork.h"
#include "state_calculators.h"
#include <stdexcept>

namespace lnet
{

BasicNetwork::BasicNetwork() :
	lsCalc_(new DefaultLinkStateCalculator(1)), lscOwn_(true)
{
}

BasicNetwork::BasicNetwork(LinkStateCalculator* lsCalc) :
	lsCalc_(lsCalc), lscOwn_(false)
{
}

BasicNetwork::BasicNetwork(const BasicNetwork& net) :
	lsCalc_(0), lscOwn_(false)
{
	if (net.lscOwn_)
	{
		// make our own default link state calculator
		lsCalc_ = new DefaultLinkStateCalculator(net.numberOfNodeStates());
		lscOwn_ = true;
	}
	else
	{
		lscOwn_ = false;
		lsCalc_ = net.lsCalc_;
	}
}

BasicNetwork::~BasicNetwork()
{
	if (lscOwn_)
		delete lsCalc_;
}

void BasicNetwork::reset(const id_size_t nNodes, const id_size_t nLinks,
		const node_state_size_t nNodeStates, LinkStateCalculator* lsCalc)
{
	if (lscOwn_)
		delete lsCalc_;
	if (lsCalc == 0)
	{
		lsCalc_ = new DefaultLinkStateCalculator(nNodeStates);
		lscOwn_ = true;
	}
	else
		lsCalc_ = lsCalc;
	doReset(nNodes, nLinks, nNodeStates);
}

void BasicNetwork::setLinkStateCalculator(LinkStateCalculator* lsCalc)
{
	if (lscOwn_)
	{
		delete lsCalc_;
		lscOwn_ = false;
	}
	if (lsCalc != 0)
	{
		if (isValidLinkStateCalculator(lsCalc))
			lsCalc_ = lsCalc;
		else
			throw(std::invalid_argument(
					"LinkStateCalculator gives impossible link states!"));
	}
	else
	{
		lsCalc_ = new DefaultLinkStateCalculator(numberOfNodeStates());
		lscOwn_ = true;
	}
	recalcLinkStates();
}

bool BasicNetwork::isValidLinkStateCalculator(LinkStateCalculator* lc) const
{
	bool retval = true;
	for (node_state_t i = 0; i < numberOfNodeStates(); ++i)
	{
		for (node_state_t j = 0; j < numberOfNodeStates(); ++j)
		{
			retval &= ((*lc)(i, j) < numberOfLinkStates());
		}
	}
	return retval;
}
}
