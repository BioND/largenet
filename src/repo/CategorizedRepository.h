/**
 * @file CategorizedRepository.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * The repo::CategorizedRepository class.
 *
 * @todo Think of more clever ways to implement consistent iterators. It does not
 * 		 seem convenient to have two types of iterators that naively do the same thing.
 * 	     We need an iterator type that
 * 		 	- is not invalidated by changing an item's category
 * 		 	- @em and iterates over items in one category efficiently.
 *			.
 * 	     Is this possible with only one iterator type?
 *
 */
#ifndef CATEGORIZEDREPOSITORY_H_
#define CATEGORIZEDREPOSITORY_H_

#include "types.h"
#include "repo_base.h"
#include "iterators.h"
#include <vector>
#include <utility>	// for std::pair
#include <iterator>
#include <exception>
#include <cassert>
#include <algorithm>

namespace repo
{

/**
 * A categorized repository of items.
 *
 * A convenient way of organizing access to objects
 * that can be grouped into a finite number of categories/classes.
 * @c CategorizedRepository features efficient access to objects and can handle some
 * additional tasks.
 *
 * @author Gerd Zschaler <zschaler@pks.mpg.de>, original concept by Thilo Gross <gross@physics.org>
 *
 * @see Class @c CRepository by Thilo Gross <gross@physics.org>.
 *
 * @todo Better remove the index operators and @c [], and rename @c item(address_t) and @c item(id_t) to
 * something clearer to prevent confusion and ambiguities.
 */
template<class T, unsigned int enlarge_factor = 10, unsigned int max_size =
		100000000>
class CategorizedRepository: public _Repo_base
{
public:
	class RepoAllocException: public std::exception
	{
	public:
		const char* what() const throw()
		{
			return "Could not resize repository!";
		}
	};

	typedef T value_type; ///< Type of stored items.
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef repo::IndexIterator iterator;
	typedef const repo::IndexIterator const_iterator;
	typedef repo::IndexIterator IndexIterator;
	typedef repo::CategoryIterator CategoryIterator;
	typedef repo::IndexIteratorRange IndexIteratorRange;
	typedef repo::CategoryIteratorRange CategoryIteratorRange;

public:
	// member functions
	/**
	 * Constructor for repository for @p cat categories.
	 * @param cat Number of categories to store. This cannot be changed later on.
	 */
	CategorizedRepository(category_t cat);
	/**
	 * Constructor for repository for @p cat categories.
	 * Space will be allocated for @p N items.
	 * @param cat Number of categories.
	 * @param N Number of items.
	 */
	CategorizedRepository(category_t cat, address_t N);
	/**
	 * Copy constructor. Creates a new repository that contains copies of
	 * all <i>valid</i> items from @p r in their respective categories.
	 * Items with invalid IDs are dropped, i.e. item IDs may differ from
	 * IDs in the original repository.
	 *
	 * @param r	CategorizedRepository to copy from
	 */
	CategorizedRepository(const CategorizedRepository& r);
	/**
	 * Destructor
	 */
	virtual ~CategorizedRepository();

	CategorizedRepository& operator=(const CategorizedRepository& r);

	/**
	 * Set number of possible categories. If @p n is smaller than the current number of
	 * categories, all items with category numbers greater or equal than @p n are put into
	 * category @p n - 1.
	 * @param n New number of categories
	 */
	void setNumberOfCategories(category_t n);

	/**
	 * Return category of item with number @p n.
	 * @param n Number of item.
	 * @return	Category ID of item.
	 */
	category_t category(address_t n) const;
	/**
	 * Return category of item with unique ID @p id.
	 * @param id Unique ID of item.
	 * @return Category ID of item.
	 */
	category_t category(id_t id) const;

	/**
	 * Set category of item with @p id.
	 * @param id Unique ID of item.
	 * @param cat Category to put item into.
	 */
	void setCategory(id_t id, category_t cat);

	/**
	 * Return item with @p id.
	 * @param id Unique ID of item
	 * @return Reference to item
	 */
	reference item(id_t id);
	const_reference item(id_t id) const;

	/**
	 * Return @p n 'th item in category @p cat
	 * @param cat Category of item
	 * @param n Number of item in category
	 * @return Reference to item
	 */
	reference item(category_t cat, address_t n);
	const_reference item(category_t cat, address_t n) const;
	/**
	 * Return ID of @p n 'th item
	 * @param n Number of item
	 * @return Unique ID of item
	 */
	id_t id(address_t n) const;
	/**
	 * Return ID of @p n 'th item in category @p cat
	 * @param cat Category of item
	 * @param n Number of item in category @p cat
	 * @return Unique ID of item
	 */
	id_t id(category_t cat, address_t n) const;

	/**
	 * Insert a copy of @p itm into category @p cat
	 * @param itm Item to insert into the repository.
	 * @param cat Category to insert the item into.
	 * @return Unique ID of inserted item.
	 */
	id_t insert(T itm, category_t cat);
	/**
	 * Insert a copy of @p itm into the repository.
	 * @param itm Item to insert into the repository.
	 * @return Unique ID of inserted item.
	 */
	id_t insert(T itm);

	/**
	 * Remove item from repository. The item is not destroyed but returned.
	 * @param id Unique ID of item to remove
	 * @return Removed item
	 */
	void remove(id_t id);

	/**
	 * Remove @p n 'th item in category @p cat from repository.
	 * The item is not destroyed but returned.
	 * @param cat Category of item
	 * @param n Number of item in category @p cat
	 * @return Removed item
	 */
	void remove(category_t cat, address_t n);

	/**
	 * Remove all items from repository. The items are not destroyed.
	 * @todo What if items are pointers? Nothing. The user must take care to
	 * properly destroy objects pointed to. Compare with STL containers (value semantics).
	 */
	void removeAll();

	/**
	 * Get item with unique ID @p id
	 * @param id Unique id of item
	 * @return Reference to item
	 */
	reference operator[](id_t id);
	const_reference operator[](id_t id) const;

	/**
	 * Insert a copy of item @p itm into the repository.
	 * @param itm Item to insert into the repository
	 * @return reference to self (for operator concatenation)
	 */
	CategorizedRepository<T, enlarge_factor, max_size>& operator<<(T itm);

	/**
	 * Return an iterator referring to the first item stored in the repository.
	 * Use this in conjunction with end() to iterate
	 * over all items. These iterators will always dereference to valid item IDs.
	 * @return Iterator pointing to first item ID.
	 */
	IndexIterator begin() const;

	/**
	 * Return an iterator referring to the past-the-end element in the repository.
	 * @return Past-the-end iterator
	 */
	IndexIterator end() const;
	/**
	 * Return iterator range of the unique IDs of all items stored in the repository.
	 * @return @c std::pair of IndexIterators, where the first points to the
	 * unique ID of the first item stored and the second to the past-the-end element
	 * in the repository.
	 */
	IndexIteratorRange ids() const;
	/**
	 * Return iterator range of the unique IDs of all items in category @p cat stored in
	 * the repository.
	 * @param cat Category to iterate over.
	 * @return @c std::pair of CategoryIterators, where the first points to the
	 * unique ID of the first item stored and the second to the past-the-end element
	 * in the repository.
	 */
	CategoryIteratorRange ids(category_t cat) const;

	void clear(); ///< clear repository

private:
	void init();
	void copyItems(const CategorizedRepository& r);
	/**
	 * Set category of item with number @p n.
	 * @param n Number of item.
	 * @param cat Category to put item into.
	 */
	void setCategory(address_t n, category_t cat);
	/**
	 * Set category of @p n 'th item in category @p oldCat to @p newCat
	 * @param oldCat Category the item is in.
	 * @param n Number of item in category @p oldCat
	 * @param newCat New category for item.
	 */
	void setCategory(category_t oldCat, address_t n, category_t newCat);

	/**
	 * Return item with number @p n.
	 * @param n Number of item
	 * @return Reference to item
	 */
	reference item(address_t n);
	const_reference item(address_t n) const;

	/**
	 * Remove @p n 'th item from repository.
	 * The item is not destroyed but returned.
	 * @param n Number of item to remove
	 * @return Removed item.
	 */
	void remove(address_t n);

	bool enlarge(); ///< enlarge the storage space
	void increaseCat(address_t n, category_t cat); ///< increase category of entry
	void decreaseCat(address_t n, category_t cat); ///< decrease category of entry

	void reorderToMaxCategory(const category_t n);

	std::vector<T> items_; ///< array of items

}; // class CategorizedRepository

// --------------- construction/destruction

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategorizedRepository(
		const category_t cat) :
	_Repo_base(cat, 100), items_(N_)
{
	init();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategorizedRepository(
		const category_t cat, const address_t N) :
	_Repo_base(cat, N), items_(N_)
{
	assert(C_> 0);
	init();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategorizedRepository(
		const CategorizedRepository<T, enlarge_factor, max_size>& r) :
	_Repo_base(r), items_(N_)
{
	init();
	copyItems(r);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::init()
{
	items_.reserve(N_);
	items_.resize(N_);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::copyItems(
		const CategorizedRepository<T, enlarge_factor, max_size>& r)
{
	assert(C_ == r.C_);
	IndexIteratorRange iters = r.ids();
	for (IndexIterator it = iters.first; it != iters.second; ++it)
	{
		insert(r.item(*it), r.category(*it));
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>& CategorizedRepository<T,
		enlarge_factor, max_size>::operator=(const CategorizedRepository<T,
		enlarge_factor, max_size>& r)
{
	C_ = r.C_;
	N_ = r.nStored_;
	_init();
	init();
	copyItems(r);
	return *this;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::~CategorizedRepository()
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setNumberOfCategories(
		const category_t n)
{
	if (n == 0)
		return;
	if (nStored_ > 0)
		reorderToMaxCategory(n - 1);
	address_t oldCCount = count_[C_];
	address_t oldCOffset = offset_[C_];
	category_t oldC = C_;
	C_ = n;
	count_.resize(C_ + 1, 0);
	offset_.resize(C_ + 1, oldCOffset);
	count_[oldC] = 0;
	count_[C_] = oldCCount;
	offset_[C_] = oldCOffset;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::reorderToMaxCategory(
		const category_t n)
{
	if (n >= C_ - 1)
		return;

	// first id to move should be ids_[offset_[n+1]], last ids_[offset_[C_]-1]
	// number of ids to move should be offset_[C_] - offset_[n+1]

	id_size_t num = offset_[C_] - offset_[n + 1];
	std::vector<id_t> temp(num, 0);
	std::copy(ids_.begin() + offset_[n + 1], ids_.begin() + offset_[C_],
			temp.begin());

	for (std::vector<id_t>::const_iterator it = temp.begin(); it != temp.end(); ++it)
	{
		decreaseCat(nums_[*it], n);
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::clear()
{
	assert(C_ > 0);
	init();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
bool CategorizedRepository<T, enlarge_factor, max_size>::enlarge()
{
	address_t newsize = (N_ > 0) ? N_ * enlarge_factor : enlarge_factor;
	if (newsize > max_size)
		newsize = max_size;
	if (newsize <= N_)
		return false;
	else
	{
		items_.reserve(newsize);
		items_.resize(newsize); // fills with default-constructed items

		ids_.reserve(newsize);
		for (address_t i = N_; i < newsize; ++i)
			ids_.push_back(i);

		nums_.reserve(newsize);
		for (id_t i = N_; i < newsize; ++i)
			nums_.push_back(i);

		count_[C_] += newsize - N_;
		N_ = newsize;
		return true;
	}
}

// --------------- category management

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::increaseCat(
		const address_t n, const category_t cat)
{
	assert(n < N_);
	assert(cat <= C_);
	category_t cls = category(n);
	address_t mynum = n;
	id_t myid = ids_[mynum];
	assert(cls <= cat);
	while (cls < cat) // Keep increasing
	{
		// First go to last pos in current group
		address_t tarnum = offset_[cls] + count_[cls] - 1;
		if (mynum != tarnum)
		{
			id_t idbuf = ids_[tarnum];
			ids_[tarnum] = ids_[mynum];
			ids_[mynum] = idbuf;
			nums_[myid] = tarnum; // This is right cause we switched nums
			nums_[idbuf] = mynum;
			mynum = tarnum;
		}
		--count_[cls]; // Now shift the boundary
		++count_[cls + 1];
		--offset_[cls + 1];
		++cls;
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::decreaseCat(
		const address_t n, const category_t cat)
{
	assert(n < N_);
	assert(cat <= C_);
	category_t cls = category(n);
	address_t mynum = n;
	id_t myid = ids_[mynum];
	assert(cls >= cat);
	while (cls > cat) // Keep decreasing
	{
		// First go to first pos in current group
		address_t tarnum = offset_[cls];
		if (mynum != tarnum)
		{
			id_t idbuf = ids_[tarnum];
			ids_[tarnum] = ids_[mynum];
			ids_[mynum] = idbuf;
			nums_[myid] = tarnum; // This is right cause we switched nums
			nums_[idbuf] = mynum;
			mynum = tarnum;
		}
		--count_[cls]; // Now shift the boundary
		++count_[cls - 1];
		++offset_[cls];
		--cls;
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline category_t CategorizedRepository<T, enlarge_factor, max_size>::category(
		const id_t id) const
{
	assert(id < N_);
	return category(nums_[id]);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline category_t CategorizedRepository<T, enlarge_factor, max_size>::category(
		const address_t n) const
{
	assert(n < N_);
	address_t na = n;
	category_t c = 0;
	while (na >= count_[c])
	{
		na -= count_[c];
		++c;
	}
	return c;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setCategory(
		const id_t id, const category_t cat)
{
	assert(id < N_);
	setCategory(nums_[id], cat);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setCategory(
		const address_t n, const category_t cat)
{
	assert(cat < C_);
	assert(n <= nStored_);
	category_t cls = category(n);
	if (cls < cat)
		increaseCat(n, cat);
	else
		decreaseCat(n, cat);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setCategory(
		const category_t oldCat, const address_t n, const category_t newCat)
{
	assert(oldCat < C_);
	assert(n < count_[oldCat]);
	assert(newCat < C_);
	address_t num = offset_[oldCat] + n;
	setCategory(num, newCat);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline/* */typename CategorizedRepository<T, enlarge_factor, max_size>::reference
CategorizedRepository<T, enlarge_factor, max_size>::item(
		const id_t id)
{
	assert(id < N_);
	assert(valid(id));
	return items_[id];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::const_reference
CategorizedRepository<T, enlarge_factor, max_size>::item(
		const id_t id) const
{
	assert(id < N_);
	assert(valid(id));
	return items_[id];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::reference
CategorizedRepository<T, enlarge_factor, max_size>::operator[](
		const id_t id)
{
	return item(id);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::const_reference
CategorizedRepository<T, enlarge_factor, max_size>::operator[](
		const id_t id) const
{
	return item(id);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::reference
CategorizedRepository<T, enlarge_factor, max_size>::item(
		const address_t n)
{
	assert(n < N_);
	assert(valid(ids_[n]));
	return items_[ids_[n]];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::const_reference
CategorizedRepository<T, enlarge_factor, max_size>::item(
		const address_t n) const
{
	assert(n < N_);
	assert(valid(ids_[n]));
	return items_[ids_[n]];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::reference
CategorizedRepository<T, enlarge_factor, max_size>::item(
		const category_t cat, const address_t n)
{
	assert(cat < C_);
	assert(n < count_[cat]);
	assert(valid(ids_[offset_[cat] + n]));
	return items_[ids_[offset_[cat] + n]];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::const_reference
CategorizedRepository<T, enlarge_factor, max_size>::item(
		const category_t cat, const address_t n) const
{
	assert(cat < C_);
	assert(n < count_[cat]);
	assert(valid(ids_[offset_[cat] + n]));
	return items_[ids_[offset_[cat] + n]];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_t CategorizedRepository<T, enlarge_factor, max_size>::id(
		const address_t n) const
{
	assert(n < N_);
	return ids_[n];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_t CategorizedRepository<T, enlarge_factor, max_size>::id(
		const category_t cat, const address_t n) const
{
	assert(cat < C_);
	assert(offset_[cat] + n < N_);
	assert(n < count_[cat]);
	return ids_[offset_[cat] + n];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::insert(const T itm,
		const category_t cat)
{
	assert(cat < C_);
	// Basic storage at the end
	address_t curnum = offset_[C_];
	if (curnum >= N_)
	{
		if (!enlarge())
		throw(RepoAllocException());
	}
	id_t uid = ids_[curnum];
	items_[uid] = itm; // Store copy of item
	decreaseCat(curnum, cat); // Move into right class
	++nStored_;
	updateMinMaxID(uid);
	return uid;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::insert(const T itm)
{
	return insert(itm, 0);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>& CategorizedRepository<T,
enlarge_factor, max_size>::operator<<(const T itm)
{
	insert(itm, 0);
	return *this;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::remove(const id_t id)
{
	assert(id < N_);
	assert(valid(id));
	return remove(nums_[id]);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::remove(
		const address_t n)
{
	assert(n < N_);
	assert(n < nStored_);
	id_t uid = ids_[n];
	items_[ids_[n]] = T(); // set to default-constructed item; thus, IDs remain unique and unchanged
	increaseCat(n, C_); // move to hidden category
	--nStored_;
	if (uid == minID_)
	updateMinID();
	if (uid == maxID_)
	updateMaxID();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::removeAll()
{
	clear();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
IndexIterator CategorizedRepository<T, enlarge_factor, max_size>::begin() const
{
	return IndexIterator(*this, minID_);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
IndexIterator CategorizedRepository<T, enlarge_factor, max_size>::end() const
{
	if (nStored_ > 0)
	return IndexIterator(*this, maxID_ + 1);
	else
	return IndexIterator(*this, maxID_);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size> IndexIteratorRange
CategorizedRepository<T, enlarge_factor, max_size>::ids() const
{
	return std::make_pair(begin(), end());
}

template<class T, unsigned int enlarge_factor, unsigned int max_size> CategoryIteratorRange
CategorizedRepository<T, enlarge_factor, max_size>::ids(
		const category_t cat) const
{
	assert(cat < C_);
	/*
	 * This is safe, because any empty items are guaranteed to be put in the
	 * private category C_. See increaseCat() and decreaseCat().
	 */
	return std::make_pair( CategoryIterator(*this, cat), CategoryIterator(*this, cat, count_[cat])
	);
}

}
#endif /* CATEGORIZEDREPOSITORY_H_ */
