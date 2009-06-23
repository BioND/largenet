/**
 * @file repo_base.h
 * @date 19.06.2009
 * @author gerd
 */

#ifndef REPO_BASE_H_
#define REPO_BASE_H_

#include "types.h"
#include <vector>
#include <cassert>

namespace repo
{

class IndexIterator;
class CategoryIterator;

class _Repo_base
{
	friend class IndexIterator;
	friend class CategoryIterator;
public:
	_Repo_base();
	_Repo_base(category_t cat);
	_Repo_base(category_t cat, id_size_t n);
	_Repo_base(const _Repo_base& r);
	virtual ~_Repo_base();

	/**
	 * Return number of items stored.
	 * @return Number of items stored.
	 */
	id_size_t count() const;
	/**
	 * Convenience function, same as count().
	 * @return Number of items stored.
	 */
	id_size_t size() const;
	/**
	 * Return number of items in category @p cat.
	 * @param cat Category to count items in.
	 * @return Number of items in category @p cat
	 */
	id_size_t count(category_t cat) const;

	/**
	 * Return current maximum number of storable items, i.e. reserved memory.
	 * @return Current capacity.
	 */
	address_t capacity() const;

	/**
	 * Get the number of categories in the repository.
	 * @return Number of categories.
	 */
	category_t numberOfCategories() const;

	/**
	 * Check if @p i is a valid ID of an item in the repository.
	 * @param i ID
	 * @return true if valid
	 */
	bool valid(id_t i) const;
	id_t maxID() const;
	id_t minID() const;

protected:
	void _init();
	void updateMinID();
	void updateMaxID();
	/**
	 * Recalculate the minimum and maximum valid ID
	 */
	void updateMinMaxID();
	/**
	 * Recalculate the minimum and maximum valid ID, where @p id is guaranteed to be valid
	 * (e. g. after insertion).
	 * @param id Valid ID
	 */
	void updateMinMaxID(id_t id);

	category_t C_; ///< number of categories
	address_t N_; ///< current max number of items
	address_t nStored_; ///< total number of items stored
	std::vector<address_t> count_; ///< number of items per category;
	std::vector<address_t> offset_; ///< start index of a class
	std::vector<address_t> nums_; ///< number of an item according to id
	std::vector<id_t> ids_; ///< id of an item according to number
	id_t minID_; ///< smallest valid ID
	id_t maxID_; ///< largest valid ID
};

inline bool _Repo_base::valid(const id_t i) const
{
	return ((nStored_ > 0) && (i < N_) && (nums_[i] < nStored_));
}

inline id_t _Repo_base::maxID() const
{
	return maxID_;
}

inline id_t _Repo_base::minID() const
{
	return minID_;
}

inline id_size_t _Repo_base::count() const
{
	return nStored_;
}

inline id_size_t _Repo_base::size() const
{
	return count();
}

inline id_size_t _Repo_base::count(const category_t cat) const
{
	assert(cat < C_);
	return count_[cat];
}

inline address_t _Repo_base::capacity() const
{
	return N_;
}

inline category_t _Repo_base::numberOfCategories() const
{
	return C_;
}

inline void _Repo_base::updateMinID()
{
	if (nStored_ > 0)
	{
		minID_ = 0;
		while (!valid(minID_) && (minID_ < N_))
			++minID_;
	}
	else
		minID_ = 0;
}

inline void _Repo_base::updateMaxID()
{
	if (nStored_ > 0)
	{
		maxID_ = N_ - 1;
		while (!valid(maxID_) && (maxID_ > 0))
			--maxID_;
	}
	else
		maxID_ = 0;
}

inline void _Repo_base::updateMinMaxID()
{
	updateMinID();
	updateMaxID();
}

inline void _Repo_base::updateMinMaxID(const id_t id)
{
	if (nStored_ > 0)
	{
		assert(id < N_);
		if (id < minID_)
			minID_ = id;
		if (id > maxID_)
			maxID_ = id;
	}
	else
	{
		minID_ = 0;
		maxID_ = 0;
	}
}

}
#endif /* REPO_BASE_H_ */
