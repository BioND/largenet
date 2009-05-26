/**
 * @file state_calculators.h
 * @date 25.05.2009
 * @author gerd
 */

#ifndef STATE_CALCULATORS_H_
#define STATE_CALCULATORS_H_

#include "types.h"
#include "../motifs/LinkMotif.h"
#include "../motifs/TripleMotif.h"

namespace lnet
{

/**
 * Functor calculating a link's state from its end nodes' states (abstract class).
 */
class LinkStateCalculator
{
public:
	/**
	 * Destructor.
	 */
	virtual ~LinkStateCalculator()
	{
	}

	link_state_t operator()(const motifs::LinkMotif& l) const
	{
		return fromLinkMotif(l);
	}

	/**
	 * Calculates link state from end node states.
	 * @param[in] source Source node's state.
	 * @param[in] target Target node's state.
	 * @return %Link state.
	 */
	link_state_t operator()(node_state_t source, node_state_t target) const
	{
		return fromNodeStates(source, target);
	}

	link_state_t fromLinkMotif(const motifs::LinkMotif& l) const
	{
		return fromNodeStates(l.left(), l.right());
	}
private:
	virtual link_state_t fromNodeStates(node_state_t source,
			node_state_t target) const = 0;
};

/**
 * Constant link state functor.
 */
template<link_state_t _ls = 0>
class ConstLinkState: public LinkStateCalculator
{
private:
	/**
	 * Returns constant link state @p _ls
	 * @param source ignored
	 * @param target ignored
	 * @return constant link state @p _ls
	 */
	link_state_t fromNodeStates(node_state_t source, node_state_t target) const
	{
		return _ls;
	}
};

/**
 * Functor calculating a triple's state from its three node's states (abstract class).
 */
class TripleStateCalculator
{
public:
	/**
	 * Destructor.
	 */
	virtual ~TripleStateCalculator()
	{
	}

	/**
	 * Calculates triple state from node states.
	 * @param left State of left node
	 * @param center State of center node
	 * @param right State of right node
	 * @return Computed triple state
	 */
	triple_state_t operator()(node_state_t left, node_state_t center,
			node_state_t right) const
	{
		return fromNodeStates(left, center, right);
	}

	triple_state_t operator()(const motifs::TripleMotif& t) const
	{
		return fromTripleMotif(t);
	}
	triple_state_t fromTripleMotif(const motifs::TripleMotif& t) const
	{
		return fromNodeStates(t.left(), t.center(), t.right());
	}
private:
	virtual triple_state_t fromNodeStates(node_state_t left,
			node_state_t center, node_state_t right) const = 0;
};

/**
 * Constant triple state functor
 */
template<triple_state_t _ts = 0>
class ConstTripleState: public TripleStateCalculator
{
private:
	/**
	 * Returns constant triple state @p _ts
	 * @param left ignored
	 * @param center ignored
	 * @param right ignored
	 * @return constant triple state @p _ts
	 */
	triple_state_t fromNodeStates(node_state_t left, node_state_t center,
			node_state_t right) const
	{
		return _ts;
	}
};

}

#endif /* STATE_CALCULATORS_H_ */
