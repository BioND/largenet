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
typedef unsigned long int id_size_t; ///< ID size type.
///@}

typedef unsigned int node_state_t; ///< %Node state type.
typedef node_state_t node_state_size_t; ///< %Node state size type.
typedef unsigned int link_state_t; ///< %Link state type.
typedef link_state_t link_state_size_t; ///< %Link state size type.

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
	virtual link_state_t operator()(const node_state_t source,
			const node_state_t target) const = 0;
};

/**
 * Constant link state functor.
 */
template<link_state_t ls = 0>
struct ConstLinkState: public LinkStateCalculator
{
	/**
	 * Returns constant link state @p ls
	 * @param source ignored
	 * @param target ignored
	 * @return constant link state @p ls
	 */
	link_state_t operator()(node_state_t source, node_state_t target) const
	{
		return ls;
	}
};

}

#endif /* TYPES_H_ */
