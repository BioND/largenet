/**
 * @file iterators.h
 * @date 19.06.2009
 * @author gerd
 */

#ifndef ITERATORS_H_
#define ITERATORS_H_

#include "types.h"
#include "repo_base.h"
#include <iterator>

namespace repo
{

/**
 * Index iterator.
 *
 * This should be used for the traversal of the repository. If valid, it will
 * always dereference to the unique ID of an item actually stored in the repository.
 * Note that inserting into the repository invalidates all iterators.
 */
class IndexIterator: public std::iterator<std::input_iterator_tag, id_t>
{
public:
	/**
	 * Default constructor needed for STL compliance. This creates a singular
	 * (invalid) iterator which does not belong to any repository.
	 */
	IndexIterator();
	/**
	 * Basic constructor. This creates an iterator pointing to the first
	 * item in the repository @p repo.
	 * @param repo Repository the iterator belongs to.
	 */
	IndexIterator(const _Repo_base& repo);
	/**
	 * Position constructor. This creates an iterator pointing to the @p n'th
	 * item in the repository @p repo.
	 * @param repo Repository the iterator belongs to.
	 * @param n Number of item.
	 */
	IndexIterator(const _Repo_base& repo, id_t n);
	/**
	 * Copy constructor.
	 * @param it Iterator to copy from.
	 */
	IndexIterator(const IndexIterator& it);
	/**
	 * Destructor. Does nothing.
	 */
	~IndexIterator();

	/**
	 * Assignment operator.
	 * @param it Iterator to assign from.
	 * @return Reference to self.
	 */
	IndexIterator& operator=(const IndexIterator& it);
	/**
	 * Equality operator.
	 * @param it Iterator to compare with.
	 * @return True if equal.
	 */
	bool operator==(const IndexIterator& it);
	/**
	 * Inequality operator.
	 * @param it Iterator to compare with.
	 * @return True if unequal.
	 */
	bool operator!=(const IndexIterator& it);
	/**
	 * Prefix increment operator.
	 * @return Reference to (incremented) self.
	 */
	IndexIterator& operator++();
	/**
	 * Postfix increment operator.
	 * @return Copy of self before increment.
	 */
	IndexIterator operator++(int);
	/**
	 * Dereference operator.
	 * @return Reference to item.
	 */
	id_t operator*();
private:
	const _Repo_base* rep_; ///< repository the iterator belongs to
	id_t cur_; ///< current iterator position
}; // class _Repo_base::IndexIterator

/**
 * Category iterator.
 *
 * Iterates over all items in one category. If valid, it will
 * always dereference to the unique ID of an item actually stored in the repository.
 * Note that in addition to insertion into the repository, <b>changing an item's category
 * invalidates this iterator</b>, too.
 */
class CategoryIterator: public std::iterator<std::input_iterator_tag, id_t>
{
public:
	/**
	 * Default constructor needed for STL compliance. This creates a singular
	 * (invalid) iterator which does not belong to any repository.
	 */
	CategoryIterator();
	/**
	 * Basic constructor. This creates an iterator pointing to the first
	 * item in category @p cat in the repository @p repo.
	 * @param repo Repository the iterator belongs to.
	 * @param cat Category to iterate over.
	 */
	CategoryIterator(const _Repo_base& repo, category_t cat);
	/**
	 * Position constructor. This creates an iterator pointing to the @p n'th
	 * item in category @p cat in the repository @p repo.
	 * @param repo Repository the iterator belongs to.
	 * @param cat Category to iterate over.
	 * @param n Number of item.
	 */
	CategoryIterator(const _Repo_base& repo, category_t cat, address_t n);
	/**
	 * Copy constructor.
	 * @param it Iterator to copy from.
	 */
	CategoryIterator(const CategoryIterator& it);
	/**
	 * Destructor. Does nothing.
	 */
	~CategoryIterator();

	/**
	 * Assignment operator.
	 * @param it Iterator to assign from.
	 * @return Reference to self.
	 */
	CategoryIterator& operator=(const CategoryIterator& it);
	/**
	 * Equality operator.
	 * @param it Iterator to compare with.
	 * @return True if equal.
	 */
	bool operator==(const CategoryIterator& it);
	/**
	 * Inequality operator.
	 * @param it Iterator to compare with.
	 * @return True if unequal.
	 */
	bool operator!=(const CategoryIterator& it);
	/**
	 * Prefix increment operator.
	 * @return Reference to (incremented) self.
	 */
	CategoryIterator& operator++();
	/**
	 * Postfix increment operator.
	 * @return Copy of self before increment.
	 */
	CategoryIterator operator++(int);
	/**
	 * Dereference operator.
	 * @return Reference to item.
	 */
	id_t operator*();
private:
	const _Repo_base* rep_; ///< Repository the iterator belongs to.
	category_t category_; ///< Category the iterator traverses.
	address_t cur_; ///< Current iterator position, relative to rep_->offset_[category_] (index in ids_ array).
}; // class _Repo_base::CategoryIterator


typedef std::pair<IndexIterator, IndexIterator> IndexIteratorRange; ///< IndexIterator range type.
typedef std::pair<CategoryIterator, CategoryIterator> CategoryIteratorRange; ///< CategoryIterator range type.


// --------------- IndexIterator

inline IndexIterator::IndexIterator() :
	rep_(0), cur_(0)
{
}

inline IndexIterator::IndexIterator(const _Repo_base& repo) :
	rep_(&repo), cur_(0)
{
	assert(rep_ != NULL);
}

inline IndexIterator::IndexIterator(const _Repo_base& repo, const id_t n) :
	rep_(&repo), cur_(n)
{
	assert(rep_ != NULL);
	// this should be correct now
	assert(cur_ <= rep_->N_);
}

inline IndexIterator::IndexIterator(const IndexIterator& it) :
	rep_(it.rep_), cur_(it.cur_)
{
}

inline IndexIterator::~IndexIterator()
{
}

inline IndexIterator& IndexIterator::operator=(const IndexIterator& it)
{
	rep_ = it.rep_;
	cur_ = it.cur_;
	return *this;
}

inline bool IndexIterator::operator==(const IndexIterator& it)
{
	return ((rep_ == it.rep_) && (cur_ == it.cur_));
}

inline bool IndexIterator::operator!=(const IndexIterator& it)
{
	return !this->operator==(it);
}

inline IndexIterator& IndexIterator::operator++()
{
	assert(rep_ != NULL);

	while (true)
	{
		if (cur_ == rep_->maxID_)
		{
			++cur_;
			break;
		}
		if (cur_ > rep_->maxID_)
			break;
		if (rep_->valid(++cur_))
			break;
	}
	return *this;
}

inline IndexIterator IndexIterator::operator++(int)
{
	IndexIterator tmp(*this);
	++(*this);
	return (tmp);
}

inline id_t IndexIterator::operator*()
{
	assert(rep_ != NULL);
	return cur_;
}

// --------------- CategoryIterator

inline CategoryIterator::CategoryIterator() :
	rep_(NULL), category_(0), cur_(0)
{
}

inline CategoryIterator::CategoryIterator(const _Repo_base& repo,
		const category_t cat) :
	rep_(&repo), category_(cat), cur_(0)
{
	assert(&repo != NULL);
	assert(cat < repo.C_);
}

inline CategoryIterator::CategoryIterator(const _Repo_base& repo,
		const category_t cat, const address_t n) :
	rep_(&repo), category_(cat), cur_(n)
{
	assert(&repo != NULL);
	assert(cat < repo.C_);
	assert(n <= repo.count_[cat]); // equal on end()
}

inline CategoryIterator::CategoryIterator(const CategoryIterator& it) :
	rep_(it.rep_), category_(it.category_), cur_(it.cur_)
{
}

inline CategoryIterator::~CategoryIterator()
{
}

inline CategoryIterator& CategoryIterator::operator=(const CategoryIterator& it)
{
	rep_ = it.rep_;
	category_ = it.category_;
	cur_ = it.cur_;
	return *this;
}

inline bool CategoryIterator::operator==(const CategoryIterator& it)
{
	return ((rep_ == it.rep_) && (category_ == it.category_) && (cur_
			== it.cur_));
}

inline bool CategoryIterator::operator!=(const CategoryIterator& it)
{
	return !this->operator==(it);
}

inline CategoryIterator& CategoryIterator::operator++()
{
	assert(rep_ != NULL);

	if (rep_)
	{
		++cur_;
	}
	return *this;
}

inline CategoryIterator CategoryIterator::operator++(int)
{
	CategoryIterator tmp(*this);
	++(*this);
	return (tmp);
}

inline id_t CategoryIterator::operator*()
{
	assert(rep_ != NULL);
	// this should be correct now
	assert(cur_ < rep_->count_[category_]);
	return rep_->ids_[cur_ + rep_->offset_[category_]];
}

}

#endif /* ITERATORS_H_ */
