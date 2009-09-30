/**
 * @file PropertyMap.h
 * @date 29.09.2009
 * @author gerd
 */

#ifndef PROPERTYMAP_H_
#define PROPERTYMAP_H_

#include "../../repo/CategorizedRepository.h"
#include <boost/call_traits.hpp>

namespace largenet
{

template<typename _id, class Property>
class PropertyMap
{
public:
	typedef typename boost::call_traits<Property>::reference property_reference;
	typedef typename boost::call_traits<Property>::const_reference
			const_property_reference;
	typedef typename boost::call_traits<Property>::value_type value_type;

public:
	PropertyMap(repo::category_t numProperties, repo::id_size_t numItems);
	virtual ~PropertyMap();

	property_reference operator[](_id id);
	const_property_reference operator[](_id id) const;

private:
	std::map<value_type, repo::category_t> propCat_;
	repo::CategorizedRepository<_id> idStore_;
};

template<typename _id, class Property>
PropertyMap<_id, Property>::PropertyMap(const repo::category_t numProperties,
		const repo::id_size_t numItems) :
	propCat_(), idStore_(numProperties, numItems)
{
}

template<typename _id, class Property>
PropertyMap<_id, Property>::~PropertyMap()
{
}

template<typename _id, class Property>
typename PropertyMap<_id, Property>::property_reference PropertyMap<_id,
		Property>::operator[](const _id id)
{
/**
 * FIXME this won't work
 *
 * We need the following behavior:
 *  - set/get property for node id (this would be a map)
 *  - get (random) node id with given property (this is CategorizedRepository, storing nodes)
 *  .
 * Using CategorizedRepository to store node IDs seems awkward and still does not do the trick,
 * as we actually mean to retrieve items by value and not by (repository) id.
 *
 * So what about storing nodes in CategorizedRepository and mapping property objects to category IDs
 * in a separate map? Here, the only inconvenience seems to be that CategorizedRepository has value
 * semantics, in contrast to boost::ptr_map, which we use currently.
 */
}

template<typename _id, class Property>
typename PropertyMap<_id, Property>::const_property_reference PropertyMap<_id,
		Property>::operator[](const _id id) const
{
}

}

#endif /* PROPERTYMAP_H_ */
