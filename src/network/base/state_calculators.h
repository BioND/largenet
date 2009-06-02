/**
 * @file state_calculators.h
 * @date 25.05.2009
 * @author gerd
 */

#ifndef STATE_CALCULATORS_H_
#define STATE_CALCULATORS_H_

#include "types.h"
#include "BasicNetwork.h"
#include "../motifs/motifs.h"
#include <map>
#include <utility>

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
	virtual ~LinkStateCalculator();
	link_state_t operator()(const motifs::LinkMotif& l) const;
	/**
	 * Calculates link state from end node states.
	 * @param[in] source Source node's state.
	 * @param[in] target Target node's state.
	 * @return %Link state.
	 */
	link_state_t operator()(node_state_t source, node_state_t target) const;
	link_state_t fromLinkMotif(const motifs::LinkMotif& l) const;

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
	link_state_t fromNodeStates(node_state_t source, node_state_t target) const;
};

/**
 * 'Canonical' link state calculator, enumerating all possible link states
 * in motif-ascending order.
 */
class DefaultLinkStateCalculator: public LinkStateCalculator
{
public:
	DefaultLinkStateCalculator(node_state_size_t nNodeStates);
private:
	link_state_t fromNodeStates(node_state_t source, node_state_t target) const;

	std::map<motifs::LinkMotif, link_state_t> states_;
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
	virtual ~TripleStateCalculator();
	/**
	 * Calculates triple state from node states.
	 * @param left State of left node
	 * @param center State of center node
	 * @param right State of right node
	 * @return Computed triple state
	 */
	triple_state_t operator()(node_state_t left, node_state_t center,
			node_state_t right) const;
	triple_state_t operator()(const motifs::TripleMotif& t) const;
	triple_state_t fromTripleMotif(const motifs::TripleMotif& t) const;
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
			node_state_t right) const;
};

class DefaultTripleStateCalculator: public TripleStateCalculator
{
public:
	DefaultTripleStateCalculator(node_state_size_t nNodeStates);
private:
	triple_state_t fromNodeStates(node_state_t left, node_state_t center,
			node_state_t right) const;
	std::map<motifs::TripleMotif, triple_state_t> states_;
};

// implementation

inline LinkStateCalculator::~LinkStateCalculator()
{
}

inline link_state_t LinkStateCalculator::operator ()(const node_state_t source,
		const node_state_t target) const
{
	return fromNodeStates(source, target);
}

inline link_state_t LinkStateCalculator::operator ()(const motifs::LinkMotif& l) const
{
	return fromLinkMotif(l);
}

inline link_state_t LinkStateCalculator::fromLinkMotif(
		const motifs::LinkMotif& l) const
{
	return fromNodeStates(l.source(), l.target());
}

// const link state

template<link_state_t _ls>
inline link_state_t ConstLinkState<_ls>::fromNodeStates(
		const node_state_t source, const node_state_t target) const
{
	return _ls;
}

// canonical link state calculator

inline DefaultLinkStateCalculator::DefaultLinkStateCalculator(
		const node_state_size_t nNodeStates)
{
	// initialize
	motifs::LinkMotifSet ls(nNodeStates);
	link_state_t i = 0;
	for (motifs::LinkMotifSet::const_iterator it = ls.begin(); it != ls.end(); ++it)
	{
		states_.insert(std::make_pair(*it, i));
		++i;
	}
}

inline link_state_t DefaultLinkStateCalculator::fromNodeStates(
		const node_state_t source, const node_state_t target) const
{
	return (states_.find(motifs::LinkMotif(source, target)))->second;
}

// triple state calculator

inline TripleStateCalculator::~TripleStateCalculator()
{
}

inline triple_state_t TripleStateCalculator::operator ()(
		const motifs::TripleMotif& t) const
{
	return fromTripleMotif(t);
}

inline triple_state_t TripleStateCalculator::operator ()(
		const node_state_t left, const node_state_t center,
		const node_state_t right) const
{
	return fromNodeStates(left, center, right);
}

inline triple_state_t TripleStateCalculator::fromTripleMotif(
		const motifs::TripleMotif& t) const
{
	return fromNodeStates(t.left(), t.center(), t.right());
}

// constant triple state

template<triple_state_t _ts>
inline triple_state_t ConstTripleState<_ts>::fromNodeStates(
		const node_state_t left, const node_state_t center,
		const node_state_t right) const
{
	return _ts;
}

// canonical triple state calculator
inline DefaultTripleStateCalculator::DefaultTripleStateCalculator(
		const node_state_size_t nNodeStates)
{
	motifs::TripleMotifSet ts(nNodeStates);
	triple_state_t i = 0;
	for (motifs::TripleMotifSet::const_iterator it = ts.begin(); it != ts.end(); ++it)
	{
		states_.insert(std::make_pair(*it, i));
		++i;
	}
}

inline triple_state_t DefaultTripleStateCalculator::fromNodeStates(
		const node_state_t left, const node_state_t center,
		const node_state_t right) const
{
	return (states_.find(motifs::TripleMotif(left, center, right)))->second;
}

}

#endif /* STATE_CALCULATORS_H_ */
