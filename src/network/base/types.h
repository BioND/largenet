/**
 * @file types.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 *
 * Type definitions for @c lnet::MultiNetwork related classes.
 */

/**
 * @namespace lnet
 * The large network namespace.
 *
 * This namespace contains everything related to the largenet package.
 */

#ifndef TYPES_H_
#define TYPES_H_

namespace lnet
{
///@{
typedef unsigned long int node_id_t; ///< %Node ID type.
typedef unsigned long int link_id_t; ///< %Link ID type.
typedef unsigned long int triple_id_t; ///< %Triple ID type.
typedef unsigned long int id_size_t; ///< ID size type.
///@}

typedef unsigned int node_state_t; ///< %Node state type.
typedef node_state_t node_state_size_t; ///< %Node state size type.
typedef unsigned int link_state_t; ///< %Link state type.
typedef link_state_t link_state_size_t; ///< %Link state size type.
typedef unsigned int triple_state_t; ///< %Triple state type.
typedef triple_state_t triple_state_size_t; ///< %Triple state size type.

/**
 * Functor calculating a link's state from its end nodes' states (abstract class).
 */
struct LinkStateCalculator
{
	/**
	 * Calculates link state from end node states.
	 * @param[in] source Source node's state.
	 * @param[in] target Target node's state.
	 * @return %Link state.
	 */
	virtual link_state_t
			operator()(node_state_t source, node_state_t target) const = 0;
};

/**
 * Constant link state functor.
 */
template<link_state_t _ls = 0>
struct ConstLinkState: public LinkStateCalculator
{
	/**
	 * Returns constant link state @p _ls
	 * @param source ignored
	 * @param target ignored
	 * @return constant link state @p _ls
	 */
	link_state_t operator()(node_state_t source, node_state_t target) const
	{
		return _ls;
	}
};

/**
 * Functor calculating a triple's state from its three node's states (abstract class).
 */
struct TripleStateCalculator
{
	/**
	 * Calculates triple state from node states.
	 * @param left State of left node
	 * @param center State of center node
	 * @param right State of right node
	 * @return Computed triple state
	 */
	virtual triple_state_t operator()(node_state_t left, node_state_t center,
			node_state_t right) const = 0;
};

/**
 * Constant triple state functor
 */
template<triple_state_t _ts = 0>
struct ConstTripleState: public TripleStateCalculator
{
	/**
	 * Returns constant triple state @p _ts
	 * @param left ignored
	 * @param center ignored
	 * @param right ignored
	 * @return constant triple state @p _ts
	 */
	triple_state_t operator()(node_state_t left, node_state_t center,
			node_state_t right) const
	{
		return _ts;
	}
};

}

#endif /* TYPES_H_ */
