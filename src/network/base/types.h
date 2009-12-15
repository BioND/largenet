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

typedef struct {} no_type;

}

#endif /* TYPES_H_ */
