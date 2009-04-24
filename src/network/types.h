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

#include <functional>

namespace lnet
{
//@{
typedef unsigned long int node_id_t; ///< %Node ID type.
typedef unsigned long int link_id_t; ///< %Link ID type.
typedef unsigned long int id_size_t; ///< ID size type.
//@}

typedef unsigned int node_state_t; ///< %Node state type.
typedef node_state_t node_state_size_t; ///< %Node state size type.
typedef unsigned int link_state_t; ///< %Link state type.
typedef link_state_t link_state_size_t; ///< %Link state size type.

class LinkStateCalculator: public std::binary_function<node_state_t,
		node_state_t, link_state_t>
{
public:
	virtual ~LinkStateCalculator() {}
	virtual link_state_t operator()(node_state_t a, node_state_t b) const = 0;
};

}

#endif /* TYPES_H_ */
