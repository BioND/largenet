/**
 * @file state_calculators.h
 * @date 25.05.2009
 * @author gerd
 */

#ifndef STATE_CALCULATORS_H_
#define STATE_CALCULATORS_H_

#include "types.h"
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
	link_state_size_t numberOfLinkStates() const;

private:
	virtual link_state_t fromNodeStates(node_state_t source,
			node_state_t target) const = 0;
	virtual link_state_size_t getNumberOfLinkStates() const = 0;
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
	link_state_size_t getNumberOfLinkStates() const;
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
	link_state_size_t getNumberOfLinkStates() const;
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
	triple_state_size_t numberOfTripleStates() const;
private:
	virtual triple_state_t fromNodeStates(node_state_t left,
			node_state_t center, node_state_t right) const = 0;
	virtual triple_state_size_t getNumberOfTripleStates() const = 0;
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
	triple_state_size_t getNumberOfTripleStates() const;
};

class DefaultTripleStateCalculator: public TripleStateCalculator
{
public:
	DefaultTripleStateCalculator(node_state_size_t nNodeStates);
private:
	triple_state_t fromNodeStates(node_state_t left, node_state_t center,
			node_state_t right) const;
	triple_state_size_t getNumberOfTripleStates() const;
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

inline link_state_size_t LinkStateCalculator::numberOfLinkStates() const
{
	return getNumberOfLinkStates();
}

// const link state

template<link_state_t _ls>
inline link_state_t ConstLinkState<_ls>::fromNodeStates(
		const node_state_t source, const node_state_t target) const
{
	return _ls;
}


template<link_state_t _ls>
inline link_state_size_t ConstLinkState<_ls>::getNumberOfLinkStates() const
{
	return 1;
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

inline link_state_size_t DefaultLinkStateCalculator::getNumberOfLinkStates() const
{
	return states_.size();
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

inline triple_state_size_t TripleStateCalculator::numberOfTripleStates() const
{
	return getNumberOfTripleStates();
}

// constant triple state

template<triple_state_t _ts>
inline triple_state_t ConstTripleState<_ts>::fromNodeStates(
		const node_state_t left, const node_state_t center,
		const node_state_t right) const
{
	return _ts;
}

template<triple_state_t _ts>
inline triple_state_size_t ConstTripleState<_ts>::getNumberOfTripleStates() const
{
	return 1;
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

inline triple_state_size_t DefaultTripleStateCalculator::getNumberOfTripleStates() const
{
	return states_.size();
}

}

#endif /* STATE_CALCULATORS_H_ */
