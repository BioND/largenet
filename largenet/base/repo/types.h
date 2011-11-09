/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */
/**
 * @file types.h
 * @author Gerd Zschaler
 * @date Aug 18, 2008
 * CategorizedRepository types.
 */
#ifndef REPO_TYPES_H_
#define REPO_TYPES_H_

/**
 * @namespace repo
 * The %CategorizedRepository namespace.
 *
 * This namespace contains everything related to the CategorizedRepository class.
 */

namespace repo
{
typedef unsigned int category_t; ///< Category number type.
typedef unsigned int address_t; ///< Item number (array index) type.
typedef unsigned long int id_t; ///< Item unique ID type.
typedef id_t id_size_t; ///< Item count type.
}

#endif /* REPO_TYPES_H_ */
